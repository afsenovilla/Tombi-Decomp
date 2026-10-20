/* --window: an SDL2 window in-process.  Every vblank the GPU hands the displayed frame (RGB24, the DISPENV
 * area) to port_window_frame, which uploads it to a streaming texture; the audio device pulls the sound
 * ring (shims/sound.c) from its callback thread; the keyboard and a game controller feed port 1 of the pad.
 * Keys: arrows = d-pad, X = cross, Z = square, D = circle, S = triangle, Q/W = L1/R1, A/E = L2/R2, Return = start,
 * Backspace = select, Escape = quit.  Closing the window ends the game. */
#include <SDL2/SDL.h>
#include "port_runtime.h"

int port_window;
int port_window_scale = 2;
static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture *tex;
static int tex_w, tex_h;
static SDL_AudioDeviceID dev;
static SDL_GameController *ctrl;
static unsigned short keys_down, ctrl_down, seen_since;

static void audio_cb(void *ud, Uint8 *stream, int len) { (void)ud; port_sound_pull((short *)stream, len / 4); }

void port_window_start(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) port_fatal("SDL_Init: %s", SDL_GetError());
    win = SDL_CreateWindow("Tombi! (port2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * port_window_scale,
                           256 * port_window_scale, SDL_WINDOW_RESIZABLE);
    if (!win) port_fatal("SDL_CreateWindow: %s", SDL_GetError());
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) ren = SDL_CreateRenderer(win, -1, 0);
    if (!ren) port_fatal("SDL_CreateRenderer: %s", SDL_GetError());
    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = 44100; want.format = AUDIO_S16SYS; want.channels = 2; want.samples = 1024; want.callback = audio_cb;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (dev) SDL_PauseAudioDevice(dev, 0); else port_printf("[port2] no audio device: %s\n", SDL_GetError());
    for (int i = 0; i < SDL_NumJoysticks(); i++)
        if (SDL_IsGameController(i) && (ctrl = SDL_GameControllerOpen(i))) { port_printf("[port2] controller: %s\n", SDL_GameControllerName(ctrl)); break; }
    port_printf("[port2] window %dx%d (scale %d)%s\n", 320 * port_window_scale, 256 * port_window_scale, port_window_scale, dev ? ", audio 44.1 kHz" : "");
}

void port_window_stop(void) {
    if (dev) SDL_CloseAudioDevice(dev);
    if (win) SDL_Quit();
    win = 0; dev = 0;
}

enum { P_SELECT = 1, P_START = 8, P_UP = 0x10, P_RIGHT = 0x20, P_DOWN = 0x40, P_LEFT = 0x80, P_L2 = 0x100, P_R2 = 0x200,
       P_L1 = 0x400, P_R1 = 0x800, P_TRI = 0x1000, P_CIRCLE = 0x2000, P_CROSS = 0x4000, P_SQUARE = 0x8000 };
static unsigned short key_mask(SDL_Scancode k) {
    switch (k) {
    case SDL_SCANCODE_UP: return P_UP; case SDL_SCANCODE_DOWN: return P_DOWN; case SDL_SCANCODE_LEFT: return P_LEFT;
    case SDL_SCANCODE_RIGHT: return P_RIGHT; case SDL_SCANCODE_RETURN: return P_START;
    case SDL_SCANCODE_BACKSPACE: case SDL_SCANCODE_RSHIFT: return P_SELECT;
    case SDL_SCANCODE_X: return P_CROSS; case SDL_SCANCODE_Z: return P_SQUARE; case SDL_SCANCODE_D: return P_CIRCLE;
    case SDL_SCANCODE_S: return P_TRI; case SDL_SCANCODE_Q: return P_L1; case SDL_SCANCODE_W: return P_R1;
    case SDL_SCANCODE_A: return P_L2; case SDL_SCANCODE_E: return P_R2;
    default: return 0;
    }
}
static unsigned short ctrl_mask(int b) {
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
static void pump(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: port_printf("[port2] window closed\n"); port_quit(0);
        case SDL_KEYDOWN:
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { port_printf("[port2] escape\n"); port_quit(0); }
            keys_down |= key_mask(e.key.keysym.scancode); seen_since |= key_mask(e.key.keysym.scancode); break;
        case SDL_KEYUP: keys_down &= ~key_mask(e.key.keysym.scancode); break;
        case SDL_CONTROLLERBUTTONDOWN: ctrl_down |= ctrl_mask(e.cbutton.button); seen_since |= ctrl_mask(e.cbutton.button); break;
        case SDL_CONTROLLERBUTTONUP: ctrl_down &= ~ctrl_mask(e.cbutton.button); break;
        case SDL_CONTROLLERDEVICEADDED: if (!ctrl) ctrl = SDL_GameControllerOpen(e.cdevice.which); break;
        }
    }
}

void port_window_frame(const unsigned char *rgb, int w, int h) {
    if (!win) return;
    pump();
    if (!tex || tex_w != w || tex_h != h) {
        if (tex) SDL_DestroyTexture(tex);
        tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, w, h);
        tex_w = w; tex_h = h;
    }
    SDL_UpdateTexture(tex, NULL, rgb, w * 3);
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);     /* stretched to the window: 512-wide modes show at the same size */
    SDL_RenderPresent(ren);
}

/* called by pad.c each vblank: every press since the previous vblank counts, so a tap is never lost */
int port_window_poll_pad(unsigned short *out) {
    if (!win) return 0;
    pump();
    *out = keys_down | ctrl_down | seen_since;
    seen_since = 0;
    return 1;
}
