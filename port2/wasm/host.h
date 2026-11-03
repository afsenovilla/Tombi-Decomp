/* port2/wasm host: what host.c, host_sdl.c and host_gl.c share (docs/PORT.md "64-bit builds: wasm2c"). */
#ifndef PORT2_WASM_HOST_H
#define PORT2_WASM_HOST_H
#include <stdint.h>
#include <string.h>
#include "tombi.h"

extern w2c_tombi port_w2c_inst;             /* the module (game thread) */
#define WMEM (port_w2c_inst.w2c_memory.data)
static inline void *wptr(u32 a) { return a ? (void *)(WMEM + a) : (void *)0; }   /* linear address -> native (0 stays NULL) */
static inline u32 waddr(const void *p) { return p ? (u32)((const uint8_t *)p - WMEM) : 0; }
static inline void wput32(u32 a, u32 v) { memcpy(WMEM + a, &v, 4); }
static inline u32 wget32(u32 a) { u32 v; memcpy(&v, WMEM + a, 4); return v; }

int port_host_on_game_thread(void);        /* the thread that runs the module (the audio callback's does not) */
void port_host_call(void (*fn)(void *), void *arg);   /* fn(arg) on the main native stack (runtime/threads.c's contract) */
void port_host_bridge_init(void);          /* host_sdl.c: the bridged variables, after instantiation */
int port_host_gl_load(void);               /* host_gl.c: GL entry points, once a context is current */
#endif
