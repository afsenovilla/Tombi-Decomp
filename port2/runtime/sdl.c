/* --window: an SDL2 window in-process.  With the OpenGL renderer (shims/gpu_gl.c, the default) the window carries a
 * GL context and the GPU backend blits the display area itself (port_window_gl_start / _size / _swap); with the
 * software rasteriser every vblank the GPU hands the displayed frame (RGB24, the DISPENV
 * area) to port_window_frame, which uploads it to a streaming texture; the audio device pulls the sound
 * ring (shims/sound.c) from its callback thread; the keyboard and a game controller feed port 1 of the pad.
 * Keys: arrows = d-pad, X = cross, Z = square, D = circle, S = triangle, Q/W = L1/R1, A/E = L2/R2, Return = start,
 * Backspace = select, F11 = fullscreen, Escape = quit (--keys up=Up,cross=X,... remaps them: SDL key names).
 * Closing the window ends the game. */
#include <string.h>
#include <SDL2/SDL.h>
#include "port_runtime.h"
#include "../shims/gpu_backend.h"

int port_window;
int port_window_scale = 2;
int port_window_fullscreen;        /* --fullscreen */
static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture *tex;
static int tex_w, tex_h;
static SDL_GLContext glctx;
static void pump(void);
static int win_hidden;             /* --headless --renderer gl: a hidden window carries the context */
static SDL_AudioDeviceID dev;
static SDL_GameController *ctrl;
static unsigned short keys_down, ctrl_down, seen_since;

/* the audio device pulls the mixer (shims/sound.c): audio time is what the device consumes, so the sequencer's
   tempo cannot depend on the game thread's pace; the game thread's calls into the mixer take the device lock */
static void audio_cb(void *ud, Uint8 *stream, int len) { (void)ud; port_sound_pull((short *)stream, len / 4); }
void port_audio_lock(void) { if (dev) SDL_LockAudioDevice(dev); }
void port_audio_unlock(void) { if (dev) SDL_UnlockAudioDevice(dev); }

void port_window_start(void) {
    SDL_SetMainReady();     /* (no SDL2main on Windows: main() is the plain C one, the launcher captures stderr) */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) port_fatal("SDL_Init: %s", SDL_GetError());
    int want_gl = port_renderer != 0;
    if (want_gl) {        /* (the SDL_Renderer of the software path is made on first use, so a failed GL start can fall back) */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    }
    win = SDL_CreateWindow("Tombi! (port2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * port_window_scale,
                           256 * port_window_scale, SDL_WINDOW_RESIZABLE | (want_gl ? SDL_WINDOW_OPENGL : 0) |
                           (port_window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    if (!win && want_gl) {
        port_printf("[port2] SDL_CreateWindow (OpenGL): %s; trying without\n", SDL_GetError());
        port_renderer = 0;
        win = SDL_CreateWindow("Tombi! (port2)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320 * port_window_scale,
                               256 * port_window_scale, SDL_WINDOW_RESIZABLE | (port_window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    }
    if (!win) port_fatal("SDL_CreateWindow: %s", SDL_GetError());
    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = 44100; want.format = AUDIO_S16SYS; want.channels = 2; want.samples = 1024; want.callback = audio_cb;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (dev) { port_sound_pulled(1); SDL_PauseAudioDevice(dev, 0); }
    else port_printf("[port2] no audio device: %s (the mixer runs per vblank, without output)\n", SDL_GetError());
    for (int i = 0; i < SDL_NumJoysticks(); i++)
        if (SDL_IsGameController(i) && (ctrl = SDL_GameControllerOpen(i))) { port_printf("[port2] controller: %s\n", SDL_GameControllerName(ctrl)); break; }
    port_printf("[port2] window %dx%d (scale %d)%s\n", 320 * port_window_scale, 256 * port_window_scale, port_window_scale, dev ? ", audio 44.1 kHz (device pulls the mixer)" : "");
}

void port_window_stop(void) {
    if (dev) { port_sound_pulled(0); SDL_CloseAudioDevice(dev); }
    if (glctx) SDL_GL_DeleteContext(glctx);
    if (win) SDL_Quit();
    win = 0; dev = 0; glctx = 0;
}

/* the OpenGL backend (shims/gpu_gl.c): a context on the window, or on a hidden one in headless mode */
int port_window_gl_start(void) {
    if (!win) {
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO) < 0) { port_printf("[port2] SDL_Init (video, for OpenGL): %s\n", SDL_GetError()); return -1; }
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        win = SDL_CreateWindow("Tombi! (port2)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64, 64, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
        if (!win) { port_printf("[port2] SDL_CreateWindow (hidden, OpenGL): %s\n", SDL_GetError()); return -1; }
        win_hidden = 1;
    }
    if (!(SDL_GetWindowFlags(win) & SDL_WINDOW_OPENGL)) return -1;
    glctx = SDL_GL_CreateContext(win);
    if (!glctx) { port_printf("[port2] SDL_GL_CreateContext: %s\n", SDL_GetError()); return -1; }
    SDL_GL_MakeCurrent(win, glctx);
    if (!win_hidden) {
        /* vsync: the swap blocks until the display's refresh; the pacing of shims/psyq.c is an absolute schedule,
           so its sleep shrinks by what the swap took (the fps line shows it as present) */
        if (SDL_GL_SetSwapInterval(1) < 0) port_printf("[port2] vsync not available: %s\n", SDL_GetError());
    }
    return 0;
}
void port_window_gl_size(int *w, int *h) { if (win) SDL_GL_GetDrawableSize(win, w, h); else *w = *h = 0; }
void port_window_gl_swap(void) {
    if (!win || win_hidden) return;
    pump();
    SDL_GL_SwapWindow(win);
}

enum { P_SELECT = 1, P_START = 8, P_UP = 0x10, P_RIGHT = 0x20, P_DOWN = 0x40, P_LEFT = 0x80, P_L2 = 0x100, P_R2 = 0x200,
       P_L1 = 0x400, P_R1 = 0x800, P_TRI = 0x1000, P_CIRCLE = 0x2000, P_CROSS = 0x4000, P_SQUARE = 0x8000 };
static const char *button_names[14] = { "select", "start", "up", "right", "down", "left", "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square" };
static const unsigned short button_bits[14] = { P_SELECT, P_START, P_UP, P_RIGHT, P_DOWN, P_LEFT, P_L2, P_R2, P_L1, P_R1, P_TRI, P_CIRCLE, P_CROSS, P_SQUARE };
static SDL_Scancode keymap[14][2] = {
    { SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_RSHIFT }, { SDL_SCANCODE_RETURN, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_UP, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_RIGHT, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_DOWN, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_LEFT, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_A, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_Q, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_S, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_D, SDL_SCANCODE_UNKNOWN },
    { SDL_SCANCODE_X, SDL_SCANCODE_UNKNOWN }, { SDL_SCANCODE_Z, SDL_SCANCODE_UNKNOWN },
};
static unsigned short key_mask(SDL_Scancode k) {
    unsigned short m = 0;
    if (k == SDL_SCANCODE_UNKNOWN) return 0;
    for (int i = 0; i < 14; i++) if (keymap[i][0] == k || keymap[i][1] == k) m |= button_bits[i];
    return m;
}
/* --keys up=Up,cross=X,...: SDL key names (SDL_GetScancodeFromName; may run before SDL_Init) */
void port_window_keys(const char *spec) {
    char buf[512]; strncpy(buf, spec, sizeof buf - 1); buf[sizeof buf - 1] = 0;
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        char *eq = strchr(tok, '=');
        if (!eq) continue;
        *eq = 0;
        SDL_Scancode sc = SDL_GetScancodeFromName(eq + 1);
        if (sc == SDL_SCANCODE_UNKNOWN) { port_printf("[port2] --keys: unknown key '%s' for %s\n", eq + 1, tok); continue; }
        for (int i = 0; i < 14; i++) if (!strcmp(tok, button_names[i])) { keymap[i][0] = sc; keymap[i][1] = SDL_SCANCODE_UNKNOWN; }
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
            if (e.key.keysym.scancode == SDL_SCANCODE_F11 && !e.key.repeat) {
                port_window_fullscreen = !port_window_fullscreen;
                SDL_SetWindowFullscreen(win, port_window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                break;
            }
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
    if (!ren) {
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!ren) ren = SDL_CreateRenderer(win, -1, 0);
        if (!ren) port_fatal("SDL_CreateRenderer: %s", SDL_GetError());
    }
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
