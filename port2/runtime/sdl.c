/* --window: an SDL2 window in-process.  With the OpenGL renderer (shims/gpu_gl.c, the default) the window carries a
 * GL context and the GPU backend blits the display area itself (port_window_gl_start / _size / _swap); with the
 * software rasteriser every vblank the GPU hands the displayed frame (RGB24, the DISPENV
 * area) to port_window_frame, which uploads it to a streaming texture; the audio device pulls the sound
 * ring (shims/sound.c) from its callback thread; the keyboard and a game controller feed port 1 of the pad.
 * Keys: arrows = d-pad, X = cross, Z = square, D = circle, S = triangle, Q/W = L1/R1, A/E = L2/R2, Return = start,
 * Backspace = select, F11 = fullscreen, Escape = quit (--keys up=Up,cross=X,... remaps them: SDL key names).
 * Game controller (SDL GameController): A = cross, B = circle, X = square, Y = triangle, LB / RB = L1 / R1, the
 * triggers (axes, past half way) = L2 / R2, Back = select, Start, the d-pad; --pad-map r1=rightshoulder,l2=b6,...
 * rebinds a PS1 button to a controller button or axis (SDL names, "lefttrigger", "leftx-"...) or to a raw joystick
 * button / axis / hat of the controller (bN, aN+ / aN-, hN.M); it replaces that button's defaults.
 * Android: TombiActivity reads the controller itself (its Android key codes / axes, the launcher's button mapping) and
 * hands the PS1 buttons over through nativePadState (registered here); SDL's controller events then stay unused.
 * Closing the window ends the game. */
#include <string.h>
#include <SDL2/SDL.h>
#include "port_runtime.h"
#include "../shims/gpu_backend.h"

#ifndef PORT2_WASM_HOST   /* (port2/wasm: these live in the wasm module, port2/wasm/sdl_bridge.h) */
int port_window;
int port_window_scale = 2;
int port_window_square = 0;                 /* 0: the 320x256 frame shows at 4:3 as on a PAL TV; 1: square pixels (5:4) */
int port_window_fullscreen;        /* --fullscreen */
#endif
#include "icon_rgba.h"
static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Texture *tex;
static int tex_w, tex_h;
static SDL_GLContext glctx;
static void pump(void);
static SDL_Window *pwin;          /* --second-screen: the panel's window (runtime/panel.c) */
static int pwin_closed;
static int win_hidden;             /* --headless --renderer gl: a hidden window carries the context */
static SDL_AudioDeviceID dev;
static SDL_GameController *ctrl;
static unsigned short keys_down, ctrl_down, seen_since;
#ifdef __ANDROID__
static volatile unsigned short java_down, java_seen;   /* TombiActivity.nativePadState */
static int java_pad;                                    /* its natives are registered: Java reads the controller */
static void android_pad_register(void);
#endif
static void ctrl_opened(void);

/* the audio device pulls the mixer (shims/sound.c): audio time is what the device consumes, so the sequencer's
   tempo cannot depend on the game thread's pace; the game thread's calls into the mixer take the device lock */
static void audio_cb(void *ud, Uint8 *stream, int len) {
    (void)ud;
#ifdef __3DS__
    { extern void port_n3ds_audio_thread_boost(void); port_n3ds_audio_thread_boost(); }   /* (n3ds/pica_c3d.c) */
#endif
    port_sound_pull((short *)stream, len / 4);
}
/* (lock_wanted: the game thread is waiting for the lock.  The 3DS's audio thread shares a throttled core and holds
   the lock for a whole callback, tens of milliseconds there: port_audio_yield, between the callback's pieces,
   lets the game thread in) */
static volatile int lock_wanted;
void port_audio_lock(void) { if (dev) { lock_wanted++; SDL_LockAudioDevice(dev); lock_wanted--; } }
void port_audio_unlock(void) { if (dev) SDL_UnlockAudioDevice(dev); }
void port_audio_yield(void) {
#ifdef __3DS__
    if (dev && lock_wanted) {
        SDL_UnlockAudioDevice(dev);
        for (int i = 0; i < 20 && lock_wanted; i++) SDL_Delay(1);
        SDL_LockAudioDevice(dev);
    }
#endif
}

/* the desktop size of display 0 (--widescreen auto); 0 = ok */
int port_display_size(int *w, int *h) {
    SDL_DisplayMode m; int r = -1;
    SDL_SetMainReady();
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 0) {
        if (SDL_GetDesktopDisplayMode(0, &m) == 0) { *w = m.w; *h = m.h; r = 0; }
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
    return r;
}

void port_window_start(void) {
    SDL_SetMainReady();     /* (no SDL2main on Windows: main() is the plain C one, the launcher captures stderr) */
    /* positional face buttons on Nintendo pads too (SDL "A" = south), so Cross is the bottom button on every pad and
       the button icons (shims/prompts.c) can name the printed label by position */
    SDL_SetHint("SDL_GAMECONTROLLER_USE_BUTTON_LABELS", "0");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) port_fatal("SDL_Init: %s", SDL_GetError());
    int want_gl = port_renderer != 0 && port_renderer != 2;   /* (2: --renderer pica, no GL context of its own) */
    if (want_gl) {        /* (the SDL_Renderer of the software path is made on first use, so a failed GL start can fall back) */
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    }
    extern int port_widescreen;
#ifndef PORT2_WASM_HOST
    extern int port_ntsc;
    int tvh = port_ntsc ? 240 : 256;       /* the NTSC picture: a 4:3 NTSC screen (240 lines, the game's 224 centred) */
#else
    int tvh = 256;
#endif
    int ww = (port_window_square ? 320 : tvh * 4 / 3) * port_window_scale;
    if (port_widescreen)        /* the same logical width port_window_fit uses, times the scale (scaling the rounded 4:3
                                   width came out a pixel short of k * lw, so the fit dropped to 1x) */
        ww = (port_window_square ? 320 + 2 * port_widescreen : tvh * 4 * (320 + 2 * port_widescreen) / (3 * 320)) * port_window_scale;
#ifdef __3DS__
    ww = 400; tvh = 240; port_window_scale = 1;   /* the top screen */
#endif
    win = SDL_CreateWindow("Tombi! (PC port)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww,
                           tvh * port_window_scale, SDL_WINDOW_RESIZABLE | (want_gl ? SDL_WINDOW_OPENGL : 0) |
                           (port_window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    if (!win && want_gl) {
        port_printf("[port2] SDL_CreateWindow (OpenGL): %s; trying without\n", SDL_GetError());
        port_renderer = 0;
        win = SDL_CreateWindow("Tombi! (PC port)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww,
                               tvh * port_window_scale, SDL_WINDOW_RESIZABLE | (port_window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
    }
    if (!win) port_fatal("SDL_CreateWindow: %s", SDL_GetError());
    {   /* the taskbar / title bar icon */
        SDL_Surface *ic = SDL_CreateRGBSurfaceFrom((void *)port_icon_rgba, 64, 64, 32, 64 * 4,
                                                   0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (ic) { SDL_SetWindowIcon(win, ic); SDL_FreeSurface(ic); }
    }
    SDL_AudioSpec want, have;
    SDL_zero(want);
    { extern int port_sound_half;
      want.freq = port_sound_half ? 22050 : 44100; want.samples = port_sound_half ? 512 : 1024; }
    want.format = AUDIO_S16SYS; want.channels = 2; want.callback = audio_cb;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
#ifdef __3DS__
    {   /* an Old 3DS (--audio-half): the mixer on its own thread on the second core (shims/sound.c, n3ds/pica_c3d.c) */
        extern int port_sound_half; extern void port_sound_mixer_run(void); extern int port_n3ds_thread_core1(void (*)(void));
        /* (not started: with the second core's share raised for it the system's own services there, the SD card and
           the GPU's, starved: loads took seconds.  The callback mixes, on the game's core, with the cheaper mixer.) */
        if (0 && dev && port_sound_half) port_n3ds_thread_core1(port_sound_mixer_run);
    }
#endif
    if (dev) { port_sound_pulled(1); SDL_PauseAudioDevice(dev, 0); }
    else port_printf("[port2] no audio device: %s (the mixer runs per vblank, without output)\n", SDL_GetError());
    for (int i = 0; i < SDL_NumJoysticks(); i++)
        if (SDL_IsGameController(i) && (ctrl = SDL_GameControllerOpen(i))) { ctrl_opened(); break; }
#ifdef __ANDROID__
    android_pad_register();
#endif
    port_printf("[port2] window %dx%d (scale %d, %s)%s\n", ww, tvh * port_window_scale, port_window_scale, port_window_square ? "square pixels" : "4:3", dev ? ", audio 44.1 kHz (device pulls the mixer)" : "");
}

static void window_stop_h(void *u) {
    (void)u;
#ifdef __3DS__
    /* (a console froze after "window closed", power button and all: each step of the way out is logged, and citro3d,
       its command queue and interrupt handlers, is closed before SDL takes the screens away under it) */
    { extern void port_n3ds_gpu_fini(void); extern void port_flush_log(void);
      port_printf("[3ds] quit: renderer\n"); port_flush_log(); port_n3ds_gpu_fini();
      port_printf("[3ds] quit: renderer closed\n"); port_flush_log();
      port_printf("[3ds] quit: audio\n"); port_flush_log();
      if (dev) { port_sound_pulled(0); SDL_CloseAudioDevice(dev); } dev = 0;
      port_printf("[3ds] quit: SDL\n"); port_flush_log();
      if (win) SDL_Quit(); win = 0;
      port_printf("[3ds] quit: done\n"); port_flush_log(); }
#endif
    if (dev) { port_sound_pulled(0); SDL_CloseAudioDevice(dev); }
    if (glctx) SDL_GL_DeleteContext(glctx);
    if (win) SDL_Quit();
    win = 0; dev = 0; glctx = 0;
}
void port_window_stop(void) { port_host_call(window_stop_h, 0); }

/* the OpenGL backend (shims/gpu_gl.c): a context on the window, or on a hidden one in headless mode */
int port_window_gl_start(void) {
    if (!win) {
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO) < 0) { port_printf("[port2] SDL_Init (video, for OpenGL): %s\n", SDL_GetError()); return -1; }
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        win = SDL_CreateWindow("Tombi! (PC port)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64, 64, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
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
        extern int port_hfr_fps;   /* --fps unlimited: no vsync, the presents go as fast as they can */
        if (SDL_GL_SetSwapInterval(port_hfr_fps == -2 ? 0 : 1) < 0) port_printf("[port2] vsync not available: %s\n", SDL_GetError());
    }
    return 0;
}
static void gl_size_h(void *a) { int **p = a; if (win) SDL_GL_GetDrawableSize(win, p[0], p[1]); else *p[0] = *p[1] = 0; }
void port_window_gl_size(int *w, int *h) { int *p[2] = { w, h }; port_host_call(gl_size_h, p); }
static void gl_swap_h(void *u) { (void)u; pump(); SDL_GL_SwapWindow(win); }
void port_window_gl_swap(void) {
    if (!win || win_hidden) return;
    port_host_call(gl_swap_h, 0);
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
/* the controller -> PS1 bindings: SDL GameController buttons / axes, or the raw joystick's buttons / axes / hats */
enum { CB_BUTTON, CB_AXIS, CB_JBUTTON, CB_JAXIS, CB_JHAT };
typedef struct { unsigned char kind, idx, dir; unsigned short bit; } CtrlBind;   /* dir: axis +1 / -1 (0 = +), hat mask */
static CtrlBind cbind[64]; static int ncbind;
static void cbind_add(int kind, int idx, int dir, unsigned short bit) { if (ncbind < 64) { CtrlBind b = { (unsigned char)kind, (unsigned char)idx, (unsigned char)dir, bit }; cbind[ncbind++] = b; } }
static void cbind_defaults(void) {
    static const struct { int b; unsigned short m; } d[] = {
        { SDL_CONTROLLER_BUTTON_DPAD_UP, P_UP }, { SDL_CONTROLLER_BUTTON_DPAD_DOWN, P_DOWN }, { SDL_CONTROLLER_BUTTON_DPAD_LEFT, P_LEFT },
        { SDL_CONTROLLER_BUTTON_DPAD_RIGHT, P_RIGHT }, { SDL_CONTROLLER_BUTTON_A, P_CROSS }, { SDL_CONTROLLER_BUTTON_B, P_CIRCLE },
        { SDL_CONTROLLER_BUTTON_X, P_SQUARE }, { SDL_CONTROLLER_BUTTON_Y, P_TRI }, { SDL_CONTROLLER_BUTTON_LEFTSHOULDER, P_L1 },
        { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, P_R1 }, { SDL_CONTROLLER_BUTTON_START, P_START }, { SDL_CONTROLLER_BUTTON_BACK, P_SELECT } };
    ncbind = 0;
    for (unsigned i = 0; i < sizeof d / sizeof d[0]; i++) {
        unsigned short m = d[i].m;
#ifdef __3DS__
        /* SDL's A / B / X / Y are the 3DS's own letters, and Nintendo's sit where the other pair does on a PlayStation
           pad: bound by place (B, the lower one, is cross; A circle; Y square; X triangle) */
        m = m == P_CROSS ? P_CIRCLE : m == P_CIRCLE ? P_CROSS : m == P_SQUARE ? P_TRI : m == P_TRI ? P_SQUARE : m;
#endif
        cbind_add(CB_BUTTON, d[i].b, 0, m);
    }
    cbind_add(CB_AXIS, SDL_CONTROLLER_AXIS_TRIGGERLEFT, 1, P_L2);    /* (L2 / R2 were not read at all before) */
    cbind_add(CB_AXIS, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 1, P_R2);
#ifdef __3DS__
    /* the Circle Pad moves as the d-pad does (the console's main stick; the game is digital) */
    cbind_add(CB_AXIS, SDL_CONTROLLER_AXIS_LEFTX, 2, P_LEFT); cbind_add(CB_AXIS, SDL_CONTROLLER_AXIS_LEFTX, 1, P_RIGHT);
    cbind_add(CB_AXIS, SDL_CONTROLLER_AXIS_LEFTY, 2, P_UP); cbind_add(CB_AXIS, SDL_CONTROLLER_AXIS_LEFTY, 1, P_DOWN);
#endif
}
/* --pad-map ps1=src[+src],...: src = an SDL controller button or axis name ("rightshoulder", "lefttrigger", "leftx-"),
   bN (raw joystick button), aN+ / aN- (raw axis), hN.M (raw hat N, direction mask M: 1 up 2 right 4 down 8 left) */
static const char *pad_map_spec;
static void cbind_apply_map(void);
void port_window_pad_map(const char *spec) {
    pad_map_spec = spec; cbind_apply_map();
    port_printf("[port2] --pad-map: %d controller bindings\n", ncbind);
}
static void cbind_apply_map(void) {
    char buf[512];
    cbind_defaults();
    if (!pad_map_spec) return;
    strncpy(buf, pad_map_spec, sizeof buf - 1); buf[sizeof buf - 1] = 0;
    for (char *tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        char *eq = strchr(tok, '='); int bi = -1;
        if (!eq) continue;
        *eq = 0;
        for (int i = 0; i < 14; i++) if (!strcmp(tok, button_names[i])) bi = i;
        if (bi < 0) { port_printf("[port2] --pad-map: unknown PS1 button '%s'\n", tok); continue; }
        for (int k = 0; k < ncbind; k++) if (cbind[k].bit == button_bits[bi]) { cbind[k--] = cbind[--ncbind]; }   /* the defaults go */
        for (char *src = eq + 1, *nx; src && *src; src = nx) {
            nx = strchr(src, '+');
            int n, m; char sign = 0;
            /* a trailing +/- of an axis is not a separator */
            if (nx && (nx[1] == 0 || nx[1] == '+' || nx[1] == ',')) { sign = '+'; *nx = 0; nx = nx[1] == '+' ? nx + 2 : NULL; }
            else if (nx) *nx++ = 0;
            size_t L = strlen(src);
            if (L && src[L - 1] == '-') { sign = '-'; src[--L] = 0; }
            SDL_GameControllerButton gb = SDL_GameControllerGetButtonFromString(src);
            SDL_GameControllerAxis ga = SDL_GameControllerGetAxisFromString(src);
            if (gb != SDL_CONTROLLER_BUTTON_INVALID) cbind_add(CB_BUTTON, gb, 0, button_bits[bi]);
            else if (ga != SDL_CONTROLLER_AXIS_INVALID) cbind_add(CB_AXIS, ga, sign == '-' ? 2 : 1, button_bits[bi]);
            else if (sscanf(src, "b%d", &n) == 1 && n >= 0 && n < 64) cbind_add(CB_JBUTTON, n, 0, button_bits[bi]);
            else if (sscanf(src, "a%d", &n) == 1 && n >= 0 && n < 16) cbind_add(CB_JAXIS, n, sign == '-' ? 2 : 1, button_bits[bi]);
            else if (sscanf(src, "h%d.%d", &n, &m) == 2 && n >= 0 && n < 4) cbind_add(CB_JHAT, n, m & 15, button_bits[bi]);
            else port_printf("[port2] --pad-map: unknown source '%s' for %s\n", src, tok);
        }
    }
}
/* the PS1 buttons held on the controller now (polled state: no event can be missed or doubled) */
static unsigned short ctrl_state(void) {
    unsigned short m = 0;
    SDL_Joystick *j = ctrl ? SDL_GameControllerGetJoystick(ctrl) : NULL;
    if (!ctrl) return 0;
    for (int k = 0; k < ncbind; k++) {
        const CtrlBind *b = &cbind[k]; int on = 0, v;
        switch (b->kind) {
        case CB_BUTTON: on = SDL_GameControllerGetButton(ctrl, (SDL_GameControllerButton)b->idx); break;
        case CB_AXIS: v = SDL_GameControllerGetAxis(ctrl, (SDL_GameControllerAxis)b->idx); on = b->dir == 2 ? v < -16384 : v > 16384; break;
        case CB_JBUTTON: on = j && b->idx < SDL_JoystickNumButtons(j) && SDL_JoystickGetButton(j, b->idx); break;
        case CB_JAXIS: if (j && b->idx < SDL_JoystickNumAxes(j)) { v = SDL_JoystickGetAxis(j, b->idx); on = b->dir == 2 ? v < -16384 : v > 16384; } break;
        case CB_JHAT: on = j && b->idx < SDL_JoystickNumHats(j) && (SDL_JoystickGetHat(j, b->idx) & b->dir) != 0; break;
        }
        if (on) m |= b->bit;
    }
    return m;
}
/* button prompts (shims/prompts.c): which physical button gives each PS1 face button with the bindings, and with
   --prompts auto the look of the controller (SDL's type); no controller: the game's own PlayStation glyphs */
extern int port_prompt_option, port_prompt_style, port_prompt_src[4], port_prompt_map_fixed;
static void prompts_update(void) {
    static const unsigned short face[4] = { P_CROSS, P_CIRCLE, P_SQUARE, P_TRI };
    if (!port_prompt_map_fixed)
        for (int f = 0; f < 4; f++) {
            int src = 8;                                       /* none */
            for (int k = 0; k < ncbind && src == 8; k++) {
                const CtrlBind *b = &cbind[k];
                if (b->bit != face[f]) continue;
                if (b->kind == CB_BUTTON) switch (b->idx) {
                    case SDL_CONTROLLER_BUTTON_A: src = 0; break; case SDL_CONTROLLER_BUTTON_B: src = 1; break;
                    case SDL_CONTROLLER_BUTTON_X: src = 2; break; case SDL_CONTROLLER_BUTTON_Y: src = 3; break;
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: src = 4; break; case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: src = 5; break;
                }
                else if (b->kind == CB_AXIS && b->idx == SDL_CONTROLLER_AXIS_TRIGGERLEFT) src = 6;
                else if (b->kind == CB_AXIS && b->idx == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) src = 7;
            }
            port_prompt_src[f] = src;
        }
    if (port_prompt_option == -1) {
        int st = 0;
        if (ctrl) switch (SDL_GameControllerGetType(ctrl)) {
            case SDL_CONTROLLER_TYPE_PS3: case SDL_CONTROLLER_TYPE_PS4: case SDL_CONTROLLER_TYPE_PS5: st = 0; break;
            case SDL_CONTROLLER_TYPE_XBOX360: case SDL_CONTROLLER_TYPE_XBOXONE: case SDL_CONTROLLER_TYPE_AMAZON_LUNA:
            case SDL_CONTROLLER_TYPE_GOOGLE_STADIA: case SDL_CONTROLLER_TYPE_NVIDIA_SHIELD: st = 1; break;
            case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO: case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
            case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT: case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR: st = 2; break;
            default: st = 3; break;                            /* virtual / unknown: the generic position diamond */
        }
        port_prompt_style = st;
    }
    if (ctrl || port_prompt_option != -1)
        port_printf("[port2] button prompts: %s (cross %d circle %d square %d triangle %d)\n",
                    (const char *[]){ "playstation", "xbox", "nintendo", "generic", "thor", "thor-black", "thor-white", "custom" }[port_prompt_style > 7 ? 3 : port_prompt_style],
                    port_prompt_src[0], port_prompt_src[1], port_prompt_src[2], port_prompt_src[3]);
}
static void ctrl_opened(void) {
    SDL_Joystick *j = SDL_GameControllerGetJoystick(ctrl);
    char *map = SDL_GameControllerMapping(ctrl);
    cbind_apply_map();
    prompts_update();
    port_printf("[port2] controller: %s (%d buttons, %d axes, %d hats; mapping %s)\n", SDL_GameControllerName(ctrl),
                j ? SDL_JoystickNumButtons(j) : -1, j ? SDL_JoystickNumAxes(j) : -1, j ? SDL_JoystickNumHats(j) : -1, map ? map : "none");
    SDL_free(map);
}
/* --dualshock: the left stick for the pad's analog bytes (shims/pad.c): a 15 % dead zone, full deflection from 90 %
   (a real stick seldom reaches the ends, where the game reads the d-pad bits) */
extern int port_dualshock, port_pad_lx, port_pad_ly;
static int stick_byte(int v) {
    float f = v / 32767.0f, a = f < 0 ? -f : f;
    if (a < 0.15f) return 0x80;
    a = (a - 0.15f) / 0.75f; if (a > 1) a = 1;
    int r = (int)(a * 127.5f + 0.5f);
    return f < 0 ? (0x80 - r < 0 ? 0 : 0x80 - r) : (0x7F + r > 0xFF ? 0xFF : 0x7F + r);
}
static void ctrl_update(void) {
    unsigned short m = ctrl_state();
    seen_since |= m & ~ctrl_down;
    ctrl_down = m;
    if (port_dualshock && ctrl) {
        port_pad_lx = stick_byte(SDL_GameControllerGetAxis(ctrl, SDL_CONTROLLER_AXIS_LEFTX));
        port_pad_ly = stick_byte(SDL_GameControllerGetAxis(ctrl, SDL_CONTROLLER_AXIS_LEFTY));
    }
}
/* the game's vibration (shims/pad.c, --dualshock): small motor on / off, big motor 0..255 */
#ifdef __ANDROID__
static void android_rumble(int small, int big);
#endif
/* --rumble N: intensity in percent (0..200, default 100; 0 = off), scaling both motors (clamped to full) */
#ifndef PORT2_WASM_HOST   /* (port2/wasm: these live in the wasm module, port2/wasm/sdl_bridge.h) */
int port_rumble_pct = 100;
#endif
static int rumble_scale(int v) { v = v * port_rumble_pct / 100; return v < 0 ? 0 : v > 255 ? 255 : v; }
typedef struct { int small, big; } RumbleArgs;
static void rumble_h(void *a) {
    RumbleArgs *r = (RumbleArgs *)a;
    int big = rumble_scale(r->big);
#ifdef __ANDROID__
    /* TombiActivity.rumble: the small motor as a level 0..255 (160 at 100 %, the device vibrator's "on") */
    if (java_pad) { android_rumble(r->small ? rumble_scale(160) : 0, big); return; }
#endif
    if (!ctrl) return;
    /* low frequency = the big motor, high frequency = the small one (full at 100 %); refreshed by every change the
       game makes, capped at 2 s in case a stop never comes */
    int small = r->small ? rumble_scale(255) : 0;
    SDL_GameControllerRumble(ctrl, (Uint16)(big * 257), (Uint16)(small * 257), small || big ? 2000 : 0);
}
void port_window_rumble(int small, int big) {
    if (!win || (port_rumble_pct <= 0 && (small || big))) return;
    RumbleArgs a = { small, big }; port_host_call(rumble_h, &a);
}

static void pump_h(void *u) { (void)u; pump(); }
static void pump(void) {
#ifdef __3DS__   /* the bottom screen's touches to the panel (n3ds/pica_c3d.c) */
    { extern void port_n3ds_touch(void); port_n3ds_touch(); }
#endif
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: port_printf("[port2] window closed\n"); port_quit(0);
        case SDL_WINDOWEVENT:   /* two windows (--second-screen): closing the game's ends the game, closing the panel's hides it */
            if (e.window.event != SDL_WINDOWEVENT_CLOSE) break;
            if (pwin && e.window.windowID == SDL_GetWindowID(pwin)) { SDL_DestroyWindow(pwin); pwin = NULL; pwin_closed = 1; break; }
            if (win && e.window.windowID == SDL_GetWindowID(win)) { port_printf("[port2] window closed\n"); port_quit(0); }
            break;
        case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP: case SDL_MOUSEMOTION: {   /* the panel window (runtime/panel.c) */
            extern void port_panel_touch(int, int, int);
            Uint32 id = e.type == SDL_MOUSEMOTION ? e.motion.windowID : e.button.windowID;
            if (!pwin || id != SDL_GetWindowID(pwin)) break;
            if (e.type == SDL_MOUSEMOTION) { if (e.motion.state & SDL_BUTTON_LMASK) port_panel_touch(e.motion.x, e.motion.y, 2); }
            else if (e.button.button == SDL_BUTTON_LEFT) port_panel_touch(e.button.x, e.button.y, e.type == SDL_MOUSEBUTTONDOWN ? 0 : 1);
            break;
        }
        case SDL_MOUSEWHEEL:   /* the panel window: continuous zoom of the map tab at the cursor (as a touch pinch) */
            if (pwin && e.wheel.windowID == SDL_GetWindowID(pwin) && e.wheel.y) {
                extern void port_panel_pinch(float, int, int);
                int mx, my; SDL_GetMouseState(&mx, &my);
                port_panel_pinch(e.wheel.y > 0 ? 1.15f : 1 / 1.15f, mx, my);
            }
            break;
        case SDL_KEYDOWN:
            if (pwin && e.key.windowID == SDL_GetWindowID(pwin)) {   /* keys typed in the panel window drive the panel, not the game */
                extern void port_panel_key(int);
                static const struct { SDL_Scancode sc; int a; } pk[] = { {SDL_SCANCODE_UP, 1}, {SDL_SCANCODE_DOWN, 2}, {SDL_SCANCODE_LEFT, 3},
                    {SDL_SCANCODE_RIGHT, 4}, {SDL_SCANCODE_RETURN, 5}, {SDL_SCANCODE_BACKSPACE, 6}, {SDL_SCANCODE_Q, 7}, {SDL_SCANCODE_E, 8},
                    {SDL_SCANCODE_PAGEDOWN, 9}, {SDL_SCANCODE_MINUS, 9}, {SDL_SCANCODE_KP_MINUS, 9}, {SDL_SCANCODE_PAGEUP, 10}, {SDL_SCANCODE_EQUALS, 10}, {SDL_SCANCODE_KP_PLUS, 10},   /* L1 / R1: map zoom */
                    {SDL_SCANCODE_H, 11}, {SDL_SCANCODE_V, 12}, {SDL_SCANCODE_P, 13} };   /* events: hide / show the selected one, list the hidden ones, pin it */
                for (unsigned i = 0; i < sizeof pk / sizeof pk[0]; i++) if (pk[i].sc == e.key.keysym.scancode) port_panel_key(pk[i].a);
                break;
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { port_printf("[port2] escape\n"); port_quit(0); }
            if (e.key.keysym.scancode == SDL_SCANCODE_F1 && !e.key.repeat) { extern int port_debug_key; port_debug_key = 1; break; }   /* --debug-menu */
            if (e.key.keysym.scancode == SDL_SCANCODE_F11 && !e.key.repeat) {
                port_window_fullscreen = !port_window_fullscreen;
                SDL_SetWindowFullscreen(win, port_window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                break;
            }
            keys_down |= key_mask(e.key.keysym.scancode); seen_since |= key_mask(e.key.keysym.scancode); break;
        case SDL_KEYUP: if (pwin && e.key.windowID == SDL_GetWindowID(pwin)) break; keys_down &= ~key_mask(e.key.keysym.scancode); break;
        case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP: case SDL_CONTROLLERAXISMOTION:
        case SDL_JOYBUTTONDOWN: case SDL_JOYBUTTONUP: case SDL_JOYAXISMOTION: case SDL_JOYHATMOTION:
            ctrl_update(); break;
        case SDL_CONTROLLERDEVICEADDED: if (!ctrl && (ctrl = SDL_GameControllerOpen(e.cdevice.which))) ctrl_opened(); break;
        case SDL_CONTROLLERDEVICEREMOVED:
            if (ctrl && e.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(ctrl))) { SDL_GameControllerClose(ctrl); ctrl = NULL; ctrl_down = 0; port_pad_lx = port_pad_ly = 0x80; prompts_update(); }
            break;
        }
    }
}

/* where the logical frame (320 wide whatever the mode, h lines) goes in a W x H output: 4:3 as a PAL TV shows
   256 lines (square pixels with port_window_square), the largest integer vertical scale when the output is within
   1/16 of it (the launcher's own window sizes), else the largest fit that keeps the aspect, centred */
#ifndef PORT2_WASM_HOST   /* (port2/wasm: these live in the wasm module, port2/wasm/sdl_bridge.h) */
int port_window_fit_wide;        /* the frame being shown is a --widescreen one (320 + 2 * pad wide) */
int port_window_fill;            /* --fit: fill the screen with a non-integer scale (keeps the aspect) */
#endif
void port_window_fit(int W, int H, int h, int *x, int *y, int *w, int *hh) {
    extern int port_widescreen;
    /* the NTSC picture's 224 lines (--picture ntsc) are 224 of the 240 a 4:3 NTSC screen shows: 320 wide either way */
    int tv = h == 224 ? 240 : h;
    int lw = port_window_square ? 320 : tv * 4 / 3;
    if (port_window_fit_wide) lw = port_window_square ? 320 + 2 * port_widescreen : tv * 4 * (320 + 2 * port_widescreen) / (3 * 320);
    int k = W / lw < H / h ? W / lw : H / h;
    float ow, oh;
    if (port_window_fill) k = 0;
    /* an integer scale only while the output is (nearly) that size: the launcher's 1x/2x/3x windows and displays that
       happen to fit. A window the player has dragged bigger, or a display between two multiples, gets the fractional
       fit instead of a picture floating in borders wider than 1/16 of it (keeps the aspect either way) */
    else if (k >= 1 && ((W - lw * k) * 16 > lw * k || (H - h * k) * 16 > h * k)) k = 0;
    if (k >= 1) { ow = (float)(lw * k); oh = (float)(h * k); }
    else { float s = (float)W / lw < (float)H / h ? (float)W / lw : (float)H / h; ow = lw * s; oh = h * s; }
    *w = (int)(ow + 0.5f); *hh = (int)(oh + 0.5f); *x = (W - *w) / 2; *y = (H - *hh) / 2;
}

/* --show-fps: the counter (runtime/fps_overlay.c) over the top-left corner of the picture, at window resolution */
static void fps_overlay_soft(int ix, int iy, int ih) {
    extern const unsigned char *port_fps_overlay(int, int *, int *, int *);
    static SDL_Texture *otex; static int ow, oh;
    int w, h, changed; const unsigned char *px = port_fps_overlay(ih, &w, &h, &changed);
    if (!px) return;
    if (!otex || ow != w || oh != h) {
        if (otex) SDL_DestroyTexture(otex);
        otex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);
        if (!otex) return;
        SDL_SetTextureBlendMode(otex, SDL_BLENDMODE_BLEND);
        ow = w; oh = h; changed = 1;
    }
    if (changed) SDL_UpdateTexture(otex, NULL, px, w * 4);
    int m = h / 4; SDL_Rect r = { ix + m, iy + m, w, h };
    SDL_RenderCopy(ren, otex, NULL, &r);
}
typedef struct { const unsigned char *px; int w, h; } FrameArgs;
static void window_frame_h(void *a) {
    const unsigned char *rgb = ((FrameArgs *)a)->px; int w = ((FrameArgs *)a)->w, h = ((FrameArgs *)a)->h;
    pump();
    if (!ren) {
        /* (the OpenGL backend presents its own frames, 24-bit ones included, through gpu_gl.c: a renderer made here
           on a window that carries its context would be a second device on the same window, on Windows a Direct3D
           one, and the picture would go to whichever surface the compositor shows last) */
        if (glctx) port_printf("[port2] note: an SDL_Renderer is created on the window that has the OpenGL context\n");
        extern int port_hfr_fps;
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | (port_hfr_fps == -2 ? 0 : SDL_RENDERER_PRESENTVSYNC));
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
    { int W, H; SDL_GetRendererOutputSize(ren, &W, &H);
      extern int port_widescreen; port_window_fit_wide = port_widescreen && w == 320 + 2 * port_widescreen;
      SDL_Rect dst; port_window_fit(W, H, h, &dst.x, &dst.y, &dst.w, &dst.h);
      SDL_RenderCopy(ren, tex, NULL, &dst);   /* 512-wide modes show at the same size */
      fps_overlay_soft(dst.x, dst.y, dst.h); }
    SDL_RenderPresent(ren);
}
void port_window_frame(const unsigned char *rgb, int w, int h) {
    if (!win) return;
    FrameArgs a = { rgb, w, h }; port_host_call(window_frame_h, &a);
}

/* called by pad.c each vblank: every press since the previous vblank counts, so a tap is never lost */
int port_window_poll_pad(unsigned short *out) {
    if (!win) return 0;
    port_host_call(pump_h, 0);
    *out = keys_down | ctrl_down | seen_since;
    seen_since = 0;
#ifdef __ANDROID__
    *out |= java_down | java_seen;
    java_seen = 0;
#endif
    return 1;
}

/* --fps auto: the display's refresh rate (Hz), 0 = unknown (no window, or the driver does not say) */
static void refresh_h(void *a) {
    SDL_DisplayMode m;
    *(int *)a = !win || win_hidden || SDL_GetWindowDisplayMode(win, &m) < 0 ? 0 : m.refresh_rate > 0 && m.refresh_rate <= 360 ? m.refresh_rate : 0;
}
int port_window_refresh(void) { int r; port_host_call(refresh_h, &r); return r; }

/* --second-screen on the PC: the panel (runtime/panel.c, RGBA) in a window of its own, through the window surface (no
   renderer: the game window may carry the OpenGL context) */
static void panel_window_frame_h(void *a) {
    const unsigned char *rgba = ((FrameArgs *)a)->px; int w = ((FrameArgs *)a)->w, h = ((FrameArgs *)a)->h;
    if (!pwin) {
        pwin = SDL_CreateWindow("Tombi! - map and status", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
        if (!pwin) { port_printf("[port2] --second-screen: SDL_CreateWindow: %s\n", SDL_GetError()); pwin_closed = 1; return; }
        SDL_RaiseWindow(win);
    }
    {   int cw, ch; SDL_GetWindowSize(pwin, &cw, &ch); if (cw != w || ch != h) SDL_SetWindowSize(pwin, w, h); }
    SDL_Surface *sf = SDL_GetWindowSurface(pwin);
    if (!sf) return;
    SDL_LockSurface(sf);
    SDL_ConvertPixels(w < sf->w ? w : sf->w, h < sf->h ? h : sf->h, SDL_PIXELFORMAT_ABGR8888, rgba, w * 4, sf->format->format, sf->pixels, sf->pitch);
    SDL_UnlockSurface(sf);
    SDL_UpdateWindowSurface(pwin);
}
void port_panel_window_frame(const unsigned char *rgba, int w, int h) {
#ifdef __3DS__   /* the bottom screen, through the citro3d renderer (n3ds/pica_c3d.c) */
    { extern void port_n3ds_panel(const unsigned char *, int, int); port_n3ds_panel(rgba, w, h); return; }
#endif
    if (!win || pwin_closed) return;
    FrameArgs a = { rgba, w, h }; port_host_call(panel_window_frame_h, &a);
}

#ifdef __ANDROID__
/* TombiActivity.nativePadState(buttons): the PS1 buttons its controller mapping holds (bits as the pad's, P_*); every
   press counts until the next vblank reads it, like the keys */
#include <jni.h>
#include <SDL2/SDL_system.h>
static void JNICALL n_pad_state(JNIEnv *env, jclass c, jint buttons) {
    (void)env; (void)c;
    unsigned short m = (unsigned short)buttons;
    java_seen |= m & ~java_down;
    java_down = m;
}
/* TombiActivity.nativePadStick(lx, ly) (--dualshock): the left stick as the pad's analog bytes, 0..255 */
static void JNICALL n_pad_stick(JNIEnv *env, jclass c, jint lx, jint ly) {
    (void)env; (void)c;
    port_pad_lx = lx < 0 ? 0 : lx > 255 ? 255 : lx; port_pad_ly = ly < 0 ? 0 : ly > 255 ? 255 : ly;
}
static void android_pad_register_h(void *u) {
    (void)u;
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    if (!env || !act) { port_printf("[port2] nativePadState: no activity\n"); return; }
    jclass cls = (*env)->GetObjectClass(env, act);
    JNINativeMethod m[] = { { "nativePadState", "(I)V", (void *)n_pad_state } };
    if ((*env)->RegisterNatives(env, cls, m, 1) != 0) { (*env)->ExceptionClear(env); port_printf("[port2] nativePadState: RegisterNatives failed (SDL reads the controller)\n"); }
    else { java_pad = 1; port_printf("[port2] nativePadState ready (TombiActivity maps the controller)\n"); }
    {   JNINativeMethod m2[] = { { "nativePadStick", "(II)V", (void *)n_pad_stick } };   /* (an older APK has no such method) */
        if ((*env)->RegisterNatives(env, cls, m2, 1) != 0) (*env)->ExceptionClear(env); }
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
}
/* TombiActivity.rumble(small, big): the controller's vibrator, or the device's (handhelds) */
static void android_rumble(int small, int big) {
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    if (!env || !act) return;
    jclass cls = (*env)->GetObjectClass(env, act);
    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "rumble", "(II)V");
    if (mid) (*env)->CallStaticVoidMethod(env, cls, mid, (jint)small, (jint)big);
    if ((*env)->ExceptionCheck(env)) (*env)->ExceptionClear(env);
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
}
static void android_pad_register(void) { port_host_call(android_pad_register_h, 0); }
#endif
