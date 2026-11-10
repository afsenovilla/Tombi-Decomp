/* Force-included before every game source of the native-pointer port (port2/tools/gen2.py). */
#ifndef PORT_PRELUDE_H
#define PORT_PRELUDE_H
#define PORT 1
#define PORT2 1
/* software GTE (port/runtime/gte.c, compiled into port2 as it is) */
void port_gte_op(unsigned long op);
void port_gte_mtc2(int reg, unsigned long v);
unsigned long port_gte_mfc2(int reg);
void port_gte_ctc2(int reg, unsigned long v);
unsigned long port_gte_cfc2(int reg);
#ifdef __3DS__
/* The 3DS: the register moves in line (the game makes ~7700 of them a frame, each a call into runtime/gte.c: with the
   register number a constant, one of these is a store or a load).  The same code as port_gte_mtc2 / mfc2 / ctc2 / cfc2. */
extern int port_gte_D[32], port_gte_C[32];
extern unsigned int port_gte_FLAG;
static inline __attribute__((always_inline)) void port_gte_mtc2_i(int r, unsigned long v) {
    int *D = port_gte_D;
    switch (r) {
    case 15: D[12] = D[13]; D[13] = D[14]; D[14] = (int)v; D[15] = (int)v; break;
    case 7: case 16: case 17: case 18: case 19: D[r] = (int)(v & 0xFFFF); break;
    case 8: case 9: case 10: case 11: D[r] = (short)v; break;
    case 28: D[28] = (int)(v & 0x7FFF);
        D[9] = (v & 0x1F) << 7; D[10] = ((v >> 5) & 0x1F) << 7; D[11] = ((v >> 10) & 0x1F) << 7; break;
    case 30: { D[30] = (int)v; int x = (int)v; unsigned u = x < 0 ? ~(unsigned)x : (unsigned)x; int n = 0;
        while (n < 32 && !(u & 0x80000000u)) { u <<= 1; n++; } D[31] = n; break; }
    case 29: case 31: break;
    default: D[r] = (int)v;
    }
}
static inline __attribute__((always_inline)) unsigned long port_gte_mfc2_i(int r) {
    int *D = port_gte_D;
    switch (r) {
    case 1: case 3: case 5: case 8: case 9: case 10: case 11: return (unsigned long)(int)(short)D[r];
    case 7: case 16: case 17: case 18: case 19: return (unsigned short)D[r];
    case 15: return (unsigned)D[14];
    case 28: case 29: {
        int r5 = D[9] >> 7, g5 = D[10] >> 7, b5 = D[11] >> 7;
        r5 = r5 < 0 ? 0 : r5 > 31 ? 31 : r5; g5 = g5 < 0 ? 0 : g5 > 31 ? 31 : g5; b5 = b5 < 0 ? 0 : b5 > 31 ? 31 : b5;
        return (unsigned)(r5 | (g5 << 5) | (b5 << 10));
    }
    default: return (unsigned)D[r];
    }
}
static inline __attribute__((always_inline)) void port_gte_ctc2_i(int r, unsigned long v) {
    if (r == 31) { port_gte_FLAG = v & 0x7FFFF000u; if (port_gte_FLAG & 0x7F87E000u) port_gte_FLAG |= 0x80000000u; return; }
    if (r == 4 || r == 12 || r == 20 || r == 26 || r == 27 || r == 29 || r == 30) v = (unsigned long)(int)(short)v;
    if (r == 26) v &= 0xFFFF;
    port_gte_C[r] = (int)v;
}
static inline __attribute__((always_inline)) unsigned long port_gte_cfc2_i(int r) { return r == 31 ? port_gte_FLAG : (unsigned)port_gte_C[r]; }
#define port_gte_mtc2(r, v) port_gte_mtc2_i(r, v)
#define port_gte_mfc2(r) port_gte_mfc2_i(r)
#define port_gte_ctc2(r, v) port_gte_ctc2_i(r, v)
#define port_gte_cfc2(r) port_gte_cfc2_i(r)
#endif
#define PORT_GTE_UNSUPPORTED(ins) port_gte_unsupported(ins)
void port_gte_unsupported(const char *ins);
void *port_bios_tcb_table(void);
/* register-carried arguments (gen_port.py forward_short_calls): the caller's incoming argument i */
#define PORT_ARG(i) (((int *)__builtin_frame_address(0))[2 + (i)])
/* An address literal of the matching sources, resolved by gen2.py to a symbol (declared in port2_syms.h as an
   unsigned char array under the C name port_a_<symbol>, which never conflicts with the game's typed externs) plus
   an offset.  The result is an integer the source then casts as it always did. */
#define PORT_ADDR(a, o) ((unsigned long)((a) + (o)))
/* Ordering-table links.  The GPU packet tag holds a 24-bit address, and the game masks pointers with 0xFFFFFF
   when it links primitives (NEXT_PRIM, compactOrderingTable's `hole`).  Phase 1 (32-bit build): the game's data
   image is linked below 16 MB (-Wl,-Ttext-segment=0x100000, checked at start-up by port_check_layout), so the
   mask is the identity on every primitive address and the link stays a plain native pointer.  Phase 2 (64-bit)
   replaces these two macros with an index into a primitive arena. */
#define port_otaddr(p) ((unsigned long)(p) & 0xFFFFFFul)
#define port_otlink(p) port_otaddr(p)
#include "port2_syms.h"
extern int port_ws_cull;          /* --widescreen: extra screen columns each side the culls keep (shims/gpu.c); 0 = off */
#define PORT_WSX(v) ((unsigned short)((v) + port_ws_cull))
#define PORT_WSW(c) ((c) + 2 * port_ws_cull)
extern int port_ws_backdrop_period(void *, int);   /* --widescreen: repeat a wrapping backdrop column (port2 main0_handwritten.c) */
/* the NTSC picture (--hz 60 unless --picture pal; docs/PORT.md "NTSC picture"): the US build's 320x224 screen.  The game
   values the PAL build changed for its 256-line picture (screen centre, camera offsets and clamps, HUD, menus...) take
   the US value through PORT_NTSC(pal, us) (port/fixes.txt) */
extern int port_ntsc;
#define PORT_NTSC(pal, us) (port_ntsc ? (us) : (pal))
#endif
