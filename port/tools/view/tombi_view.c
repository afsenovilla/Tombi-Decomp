/* tombi-view: the SDL2 window of the port (`tombi --window`).  A separate host-native (64-bit) process, because the
 * game binary is i386 and freestanding; see port/runtime/window.c for the protocol:
 *   stdin : frames "TBFR" u32 w, u32 h, w*h RGB24 bytes
 *   fd 3  : audio, s16le stereo 44100 Hz
 *   fd 4  : we write one u16 per frame: pad 1 buttons in PSX bit order, active high
 * Keyboard: arrows = d-pad, X = Cross, Z = Square, S = Triangle, D = Circle, Q/W = L1/R1, A/E = L2/R2,
 * Return = Start, Backspace or Right Shift = Select, F = toggle 2x/3x window scale, F11 = fullscreen, Escape = quit.
 * A game controller (SDL_GameController) maps to the same buttons.  Can also be used alone on a recording:
 *   build/port/tombi --frames 1200 --pad-script 1000:start | build/port/tombi-view   (no audio / pad pipes).
 * Options (tombi --viewer-arg X passes them through): --scale 1|2|3  --fullscreen
 *   --keys up=Up,down=Down,left=Left,right=Right,cross=X,square=Z,triangle=S,circle=D,l1=Q,r1=W,l2=A,r2=E,
 *          start=Return,select=Backspace   (SDL key names, SDL_GetScancodeFromName; unnamed buttons keep the default)
 * Windows: the audio and pad pipes cannot be fds 3 and 4; the game passes their handles as
 *   --audio-handle N --pad-handle N (runtime/sys_win32_i386.c), and stdin is read with ReadFile. */
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
typedef HANDLE pipe_t;
#define NO_PIPE NULL
static long rd(pipe_t h, void *buf, size_t n) { DWORD got = 0; if (!ReadFile(h, buf, (DWORD)n, &got, NULL)) return GetLastError() == ERROR_BROKEN_PIPE ? 0 : -1; return (long)got; }
static long wr(pipe_t h, const void *buf, size_t n) { DWORD put = 0; if (!WriteFile(h, buf, (DWORD)n, &put, NULL)) return -1; return (long)put; }
#else
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
typedef int pipe_t;
#define NO_PIPE (-1)
static long rd(pipe_t fd, void *buf, size_t n) { ssize_t r; do r = read(fd, buf, n); while (r < 0 && errno == EINTR); return (long)r; }
static long wr(pipe_t fd, const void *buf, size_t n) { ssize_t r = write(fd, buf, n); return r < 0 && errno == EAGAIN ? (long)n : (long)r; }
#endif

static int read_all(pipe_t fd, void *buf, size_t n) {
    char *c = buf;
    while (n) {
        long r = rd(fd, c, n);
        if (r <= 0) return -1;
        c += r; n -= r;
    }
    return 0;
}

static pipe_t audio_fd = NO_PIPE;
static SDL_AudioDeviceID dev;
/* The game produces samples on its own 44.1 kHz clock (shims/sound.c), so the queue only has to absorb jitter: it is
   held between about two and four frames (40-80 ms at 50 Hz).  Primed with two frames of silence; when it runs dry
   SDL plays silence; when it is over the ceiling the thread stops reading, the pipe fills and the game's write
   blocks, so the game slows down.  Audio is never resampled or dropped: that would change the pitch or the tempo. */
#define FRAME_BYTES (44100 * 4 / 50)
static int audio_thread(void *arg) {
    (void)arg;
    unsigned char buf[FRAME_BYTES];
    if (dev) { memset(buf, 0, sizeof buf); SDL_QueueAudio(dev, buf, sizeof buf); SDL_QueueAudio(dev, buf, sizeof buf); }
    for (;;) {
        while (dev && SDL_GetQueuedAudioSize(dev) > 4 * FRAME_BYTES) SDL_Delay(2);
        long r = rd(audio_fd, buf, sizeof buf);
        if (r <= 0) break;
        if (dev) SDL_QueueAudio(dev, buf, (Uint32)r);
    }
    return 0;
}

enum { P_SELECT = 1, P_START = 8, P_UP = 0x10, P_RIGHT = 0x20, P_DOWN = 0x40, P_LEFT = 0x80, P_L2 = 0x100, P_R2 = 0x200,
       P_L1 = 0x400, P_R1 = 0x800, P_TRI = 0x1000, P_CIRCLE = 0x2000, P_CROSS = 0x4000, P_SQUARE = 0x8000 };

/* key map: two scancodes per button (the second is a fixed alternative) */
static const char *const button_names[14] = { "select", "start", "up", "right", "down", "left", "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square" };
static const Uint16 button_bits[14] = { P_SELECT, P_START, P_UP, P_RIGHT, P_DOWN, P_LEFT, P_L2, P_R2, P_L1, P_R1, P_TRI, P_CIRCLE, P_CROSS, P_SQUARE };
static SDL_Scancode keymap[14][2] = {
    { SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_RSHIFT }, { SDL_SCANCODE_RETURN, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_UP, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_RIGHT, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_DOWN, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_LEFT, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_A, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_Q, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_S, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_D, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_X, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_Z, SDL_SCANCODE_UNKNOWN },
};
static Uint16 key_mask(SDL_Scancode k) {
    Uint16 m = 0;
    if (k == SDL_SCANCODE_UNKNOWN) return 0;
    for (int i = 0; i < 14; i++) if (keymap[i][0] == k || keymap[i][1] == k) m |= button_bits[i];
    return m;
}
/* --keys up=Up,cross=X,...: SDL key names (SDL_GetScancodeFromName) */
static void parse_keys(const char *spec) {
    char buf[512]; strncpy(buf, spec, sizeof buf - 1); buf[sizeof buf - 1] = 0;
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        char *eq = strchr(tok, '=');
        if (!eq) continue;
        *eq = 0;
        SDL_Scancode sc = SDL_GetScancodeFromName(eq + 1);
        if (sc == SDL_SCANCODE_UNKNOWN) { fprintf(stderr, "tombi-view: unknown key '%s' for %s\n", eq + 1, tok); continue; }
        for (int i = 0; i < 14; i++) if (!strcmp(tok, button_names[i])) { keymap[i][0] = sc; keymap[i][1] = SDL_SCANCODE_UNKNOWN; }
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
    int scale = 2, fullscreen = 0;
    pipe_t pad_fd = NO_PIPE, video_fd;
#ifdef _WIN32
    video_fd = GetStdHandle(STD_INPUT_HANDLE);
#else
    video_fd = 0;
    pad_fd = fcntl(4, F_GETFD) >= 0 ? 4 : -1;
    audio_fd = fcntl(3, F_GETFD) >= 0 ? 3 : -1;
#endif
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--scale") && i + 1 < argc) { scale = atoi(argv[++i]); if (scale < 1) scale = 1; if (scale > 6) scale = 6; }
        else if (!strcmp(argv[i], "--fullscreen")) fullscreen = 1;
        else if (!strcmp(argv[i], "--keys") && i + 1 < argc) parse_keys(argv[++i]);
#ifdef _WIN32
        else if (!strcmp(argv[i], "--audio-handle") && i + 1 < argc) audio_fd = (HANDLE)(uintptr_t)strtoul(argv[++i], NULL, 10);
        else if (!strcmp(argv[i], "--pad-handle") && i + 1 < argc) pad_fd = (HANDLE)(uintptr_t)strtoul(argv[++i], NULL, 10);
#endif
        else fprintf(stderr, "tombi-view: unknown option %s\n", argv[i]);
    }
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        fprintf(stderr, "tombi-view: SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("Tombi! (port)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       320 * scale, 256 * scale, SDL_WINDOW_RESIZABLE | (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) ren = SDL_CreateRenderer(win, -1, 0);
    if (!win || !ren) { fprintf(stderr, "tombi-view: %s\n", SDL_GetError()); return 1; }
    SDL_Texture *tex = NULL; int tw = 0, th = 0;

    if (audio_fd != NO_PIPE) {
        SDL_AudioSpec want, have;
        memset(&want, 0, sizeof want);
        want.freq = 44100; want.format = AUDIO_S16LSB; want.channels = 2; want.samples = 512;
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
                } else if (e.key.keysym.scancode == SDL_SCANCODE_F11 && !e.key.repeat) {
                    fullscreen = !fullscreen;
                    SDL_SetWindowFullscreen(win, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
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
        if (read_all(video_fd, hdr, 12) < 0) break;
        if (hdr[0] != 0x52464254u || hdr[1] == 0 || hdr[1] > 1024 || hdr[2] == 0 || hdr[2] > 512) {
            fprintf(stderr, "tombi-view: bad frame header\n"); break;
        }
        int w = (int)hdr[1], h = (int)hdr[2];
        size_t n = (size_t)w * h * 3;
        if (n > rgb_cap) { rgb = realloc(rgb, n); rgb_cap = n; }
        if (read_all(video_fd, rgb, n) < 0) break;
        if (!tex || tw != w || th != h) {
            if (tex) SDL_DestroyTexture(tex);
            tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, w, h);
            tw = w; th = h;
            /* keep the PAL 4:3 look: 320x256 and 512x256 both fill the same window */
            if (!fullscreen) SDL_SetWindowSize(win, 320 * scale, 256 * scale);
        }
        SDL_UpdateTexture(tex, NULL, rgb, w * 3);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        if (pad_fd != NO_PIPE) {
            Uint16 p = keys | buttons | tapped;
            tapped = 0;
            if (wr(pad_fd, &p, 2) != 2) break;
        }
    }
    SDL_Quit();
    return 0;
}
