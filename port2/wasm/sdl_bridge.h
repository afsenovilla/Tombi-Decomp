/* Force-included (-include) when runtime/sdl.c is compiled natively for the wasm2c host (port2/wasm): sdl.c stays the
 * same file as in the 32-bit builds, but the runtime variables it shares with the game (bridge_vars.h) live in the
 * module's linear memory.  Every use of such a name becomes (*port_hv_NAME), a pointer the host sets after the
 * module is instantiated (host_sdl.c), so `port_renderer = 0`, `extern int port_widescreen;` and
 * `port_prompt_src[f]` keep their meaning.  sdl.c leaves out its own definitions of them under PORT2_WASM_HOST. */
#ifndef PORT2_SDL_BRIDGE_H
#define PORT2_SDL_BRIDGE_H
#define PORT2_WASM_HOST 1
#include "bridge_vars.h"
#define PORT_HV_DECL(name) extern int *port_hv_##name;
PORT_WASM_BRIDGE_VARS(PORT_HV_DECL)
extern int (*port_hv_port_prompt_src)[4];
#define port_window (*port_hv_port_window)
#define port_window_scale (*port_hv_port_window_scale)
#define port_window_square (*port_hv_port_window_square)
#define port_window_fullscreen (*port_hv_port_window_fullscreen)
#define port_window_fit_wide (*port_hv_port_window_fit_wide)
#define port_window_fill (*port_hv_port_window_fill)
#define port_rumble_pct (*port_hv_port_rumble_pct)
#define port_renderer (*port_hv_port_renderer)
#define port_widescreen (*port_hv_port_widescreen)
#define port_hfr_fps (*port_hv_port_hfr_fps)
#define port_prompt_option (*port_hv_port_prompt_option)
#define port_prompt_style (*port_hv_port_prompt_style)
#define port_prompt_map_fixed (*port_hv_port_prompt_map_fixed)
#define port_dualshock (*port_hv_port_dualshock)
#define port_pad_lx (*port_hv_port_pad_lx)
#define port_pad_ly (*port_hv_port_pad_ly)
#define port_debug_key (*port_hv_port_debug_key)
#define port_frame (*port_hv_port_frame)
#define port_exit_status (*port_hv_port_exit_status)
#define port_clean_exit (*port_hv_port_clean_exit)
#define port_prompt_src (*port_hv_port_prompt_src)
#endif
