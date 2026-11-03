/* port2/wasm: the runtime variables that the native host code (runtime/sdl.c compiled for the host, the Android loader)
 * reads or writes although they live in the wasm module's linear memory.  rt_wasm.c returns their linear addresses
 * (port_wasm_var(i), in this order); sdl_bridge.h turns every use in the host's sdl.c into a load / store through a
 * pointer into linear memory.  All are int; port_prompt_src (int[4]) is the last entry. */
#ifndef PORT2_WASM_BRIDGE_VARS_H
#define PORT2_WASM_BRIDGE_VARS_H
#define PORT_WASM_BRIDGE_VARS(X) \
    X(port_window) X(port_window_scale) X(port_window_square) X(port_window_fullscreen) X(port_window_fit_wide) \
    X(port_window_fill) X(port_rumble_pct) X(port_renderer) X(port_widescreen) X(port_hfr_fps) X(port_prompt_option) \
    X(port_prompt_style) X(port_prompt_map_fixed) X(port_dualshock) X(port_pad_lx) X(port_pad_ly) X(port_debug_key) \
    X(port_frame) X(port_exit_status) X(port_clean_exit)
#define PORT_WASM_VAR_ID(name) PORT_WASM_VAR_##name,
enum { PORT_WASM_BRIDGE_VARS(PORT_WASM_VAR_ID) PORT_WASM_VAR_port_prompt_src, PORT_WASM_VAR_COUNT };
#endif
