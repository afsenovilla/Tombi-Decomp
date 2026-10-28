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
#endif
