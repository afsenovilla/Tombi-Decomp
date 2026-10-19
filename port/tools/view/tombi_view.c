/* tombi-view: the SDL2 window of the port (`tombi --window`).  A separate host-native (64-bit) process, because the
 * game binary is i386 and freestanding; see port/runtime/window.c for the protocol:
 *   stdin : frames "TBFR" u32 w, u32 h, w*h RGB24 bytes
 *   fd 3  : audio, s16le stereo 44100 Hz
 *   fd 4  : we write one u16 per frame: pad 1 buttons in PSX bit order, active high
 * Keyboard: arrows = d-pad, X = Cross, Z = Square, S = Triangle, D = Circle, Q/W = L1/R1, A/E = L2/R2,
 * Return = Start, Backspace or Right Shift = Select, F = toggle 2x/3x window scale, Escape = quit.
 * A game controller (SDL_GameController) maps to the same buttons.  Can also be used alone on a recording:
 *   build/port/tombi --frames 1200 --pad-script 1000:start | build/port/tombi-view   (no audio / pad pipes). */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static int read_all(int fd, void *buf, size_t n) {
    char *c = buf;
    while (n) {
        ssize_t r = read(fd, c, n);
        if (r < 0) { if (errno == EINTR) continue; return -1; }
        if (r == 0) return -1;
        c += r; n -= r;
    }
    return 0;
}

static int audio_fd = -1;
static SDL_AudioDeviceID dev;
static int audio_thread(void *arg) {
    (void)arg;
    unsigned char buf[4096];
    for (;;) {
        ssize_t r = read(audio_fd, buf, sizeof buf);
        if (r < 0) { if (errno == EINTR) continue; break; }
        if (r == 0) break;
        if (dev) {
            /* keep the queue short (about 100 ms): the game runs at 50 Hz, SDL's clock drifts slightly */
            if (SDL_GetQueuedAudioSize(dev) > 44100 * 4 / 10) continue;
            SDL_QueueAudio(dev, buf, (Uint32)r);
        }
    }
    return 0;
}

enum { P_SELECT = 1, P_START = 8, P_UP = 0x10, P_RIGHT = 0x20, P_DOWN = 0x40, P_LEFT = 0x80, P_L2 = 0x100, P_R2 = 0x200,
       P_L1 = 0x400, P_R1 = 0x800, P_TRI = 0x1000, P_CIRCLE = 0x2000, P_CROSS = 0x4000, P_SQUARE = 0x8000 };

static Uint16 key_mask(SDL_Scancode k) {
    switch (k) {
    case SDL_SCANCODE_UP: return P_UP; case SDL_SCANCODE_DOWN: return P_DOWN;
    case SDL_SCANCODE_LEFT: return P_LEFT; case SDL_SCANCODE_RIGHT: return P_RIGHT;
    case SDL_SCANCODE_X: return P_CROSS; case SDL_SCANCODE_Z: return P_SQUARE;
    case SDL_SCANCODE_S: return P_TRI; case SDL_SCANCODE_D: return P_CIRCLE;
    case SDL_SCANCODE_Q: return P_L1; case SDL_SCANCODE_W: return P_R1;
    case SDL_SCANCODE_A: return P_L2; case SDL_SCANCODE_E: return P_R2;
    case SDL_SCANCODE_RETURN: return P_START;
    case SDL_SCANCODE_BACKSPACE: case SDL_SCANCODE_RSHIFT: return P_SELECT;
    default: return 0;
    }
}
static Uint16 ctrl_mask(SDL_GameControllerButton b) {
    switch (b) {
    case SDL_CONTROLLER_BUTTON_DPAD_UP: return P_UP; case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return P_DOWN;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return P_LEFT; case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return P_RIGHT;
    case SDL_CONTROLLER_BUTTON_A: return P_CROSS; case SDL_CONTROLLER_BUTTON_B: return P_CIRCLE;
    case SDL_CONTROLLER_BUTTON_X: return P_SQUARE; case SDL_CONTROLLER_BUTTON_Y: return P_TRI;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return P_L1; case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return P_R1;
    case SDL_CONTROLLER_BUTTON_START: return P_START; case SDL_CONTROLLER_BUTTON_BACK: return P_SELECT;
    default: return 0;
    }
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    int pad_fd = fcntl(4, F_GETFD) >= 0 ? 4 : -1;
    audio_fd = fcntl(3, F_GETFD) >= 0 ? 3 : -1;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "tombi-view: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    int scale = 2;
    SDL_Window *win = SDL_CreateWindow("Tombi! (port)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       320 * scale, 256 * scale, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) ren = SDL_CreateRenderer(win, -1, 0);
    if (!win || !ren) { fprintf(stderr, "tombi-view: %s\n", SDL_GetError()); return 1; }
    SDL_Texture *tex = NULL; int tw = 0, th = 0;

    if (audio_fd >= 0) {
        SDL_AudioSpec want, have;
        memset(&want, 0, sizeof want);
        want.freq = 44100; want.format = AUDIO_S16LSB; want.channels = 2; want.samples = 1024;
        dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
        if (dev) SDL_PauseAudioDevice(dev, 0); else fprintf(stderr, "tombi-view: no audio device: %s\n", SDL_GetError());
        SDL_CreateThread(audio_thread, "audio", NULL);
    }
    SDL_GameController *ctrl = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); i++) if (SDL_IsGameController(i)) { ctrl = SDL_GameControllerOpen(i); break; }

    Uint16 keys = 0, buttons = 0, tapped = 0;   /* tapped: pressed since the last sample, so a tap shorter than a frame is still sent once */
    unsigned char *rgb = NULL; size_t rgb_cap = 0;
    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: running = 0; break;
            case SDL_KEYDOWN:
                if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) running = 0;
                else if (e.key.keysym.scancode == SDL_SCANCODE_F && !e.key.repeat) {
                    scale = scale == 2 ? 3 : 2;
                    SDL_SetWindowSize(win, (tw ? tw : 320) * scale, (th ? th : 256) * scale);
                } else { keys |= key_mask(e.key.keysym.scancode); tapped |= key_mask(e.key.keysym.scancode); }
                break;
            case SDL_KEYUP: keys &= ~key_mask(e.key.keysym.scancode); break;
            case SDL_CONTROLLERBUTTONDOWN: buttons |= ctrl_mask(e.cbutton.button); tapped |= ctrl_mask(e.cbutton.button); break;
            case SDL_CONTROLLERBUTTONUP: buttons &= ~ctrl_mask(e.cbutton.button); break;
            case SDL_CONTROLLERAXISMOTION:
                if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) { if (e.caxis.value > 16000) buttons |= P_L2; else buttons &= ~P_L2; }
                if (e.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) { if (e.caxis.value > 16000) buttons |= P_R2; else buttons &= ~P_R2; }
                if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                    buttons &= ~(P_LEFT | P_RIGHT);
                    if (e.caxis.value < -16000) buttons |= P_LEFT; else if (e.caxis.value > 16000) buttons |= P_RIGHT;
                }
                if (e.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                    buttons &= ~(P_UP | P_DOWN);
                    if (e.caxis.value < -16000) buttons |= P_UP; else if (e.caxis.value > 16000) buttons |= P_DOWN;
                }
                break;
            case SDL_CONTROLLERDEVICEADDED: if (!ctrl) ctrl = SDL_GameControllerOpen(e.cdevice.which); break;
            }
        }
        if (!running) break;
        /* one frame from the game (blocks for at most one vblank at 50 Hz) */
        Uint32 hdr[3];
        if (read_all(0, hdr, 12) < 0) break;
        if (hdr[0] != 0x52464254u || hdr[1] == 0 || hdr[1] > 1024 || hdr[2] == 0 || hdr[2] > 512) {
            fprintf(stderr, "tombi-view: bad frame header\n"); break;
        }
        int w = (int)hdr[1], h = (int)hdr[2];
        size_t n = (size_t)w * h * 3;
        if (n > rgb_cap) { rgb = realloc(rgb, n); rgb_cap = n; }
        if (read_all(0, rgb, n) < 0) break;
        if (!tex || tw != w || th != h) {
            if (tex) SDL_DestroyTexture(tex);
            tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, w, h);
            tw = w; th = h;
            /* keep the PAL 4:3 look: 320x256 and 512x256 both fill the same window */
            SDL_SetWindowSize(win, 320 * scale, 256 * scale);
        }
        SDL_UpdateTexture(tex, NULL, rgb, w * 3);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        if (pad_fd >= 0) {
            Uint16 p = keys | buttons | tapped;
            tapped = 0;
            if (write(pad_fd, &p, 2) != 2 && errno != EAGAIN) break;
        }
    }
    SDL_Quit();
    return 0;
}
