/* X000 functions the decomp has no C for, translated by hand for the port (as main0_handwritten.c does for MAIN0).
 *
 * func_80132BB8 / func_80132DD0 (0x218 / 0x1B0 bytes, read from the X000.BIN disassembly): the subtype 1 and 2
 * handlers of A00_WOODEN_BRANCH_HANDLERS (0x801392E0), called by updateA00WoodenBranch (FUN_801328b4) through
 * the table.  Ghidra never listed them (they are reached only through the table), so notes/functions_x000.csv had
 * a 0x3C8-byte gap at 0x80132BB8 and the relocator made the two table words plain data pointers: the first
 * wooden branch of subtype 1 or 2 jumped into the data image.  Registered like the MAIN0 ones: gen_port.py puts a
 * `func_XXXXXXXX(` definition of port2/runtime into the function map of the X000 unit at that address.
 *
 * Both are a four-state step on the object: state 0 waits for b69 (the requested bend direction, 2 or 3) and
 * copies it to b6b; state 1 counts w20 up by 4 to 32, every step moving the branch's own and its linked objects'
 * (d90, and d94 for the first one) edge pairs (wb6/wc6 for direction 2, wbe/wce for 3) by one; state 2 goes to
 * 3; state 3 counts w20 down and moves them back every 4th step, then returns to state 0. */
#include "port_runtime.h"

typedef unsigned char u8; typedef short s16; typedef unsigned short u16; typedef unsigned int u32;
#define F8(o, off)  (*(u8 *)((u8 *)(o) + (off)))
#define S16(o, off) (*(s16 *)((u8 *)(o) + (off)))
#define F16(o, off) (*(u16 *)((u8 *)(o) + (off)))
#define P32(o, off) (*(u8 **)((u8 *)(o) + (off)))

#define ST   0x05      /* step state */
#define W20  0x20      /* bend counter */
#define B69  0x69      /* bend request (2 / 3) */
#define B6B  0x6B      /* bend direction being undone */
#define D90  0x90      /* linked object */
#define D94  0x94      /* second linked object (func_80132BB8 only) */

static void bend(u8 *o, u8 *p3, u8 *p5, int dir, int d)
{
    if (dir == 2) {
        F16(p3, 0xC6) += d;
        F16(o, 0xB6) += d; F16(o, 0xC6) += d;
        if (p5) { F16(p5, 0xB6) += d; F16(p5, 0xC6) += d; }
    } else if (dir == 3) {
        F16(p3, 0xCE) += d;
        F16(o, 0xBE) += d; F16(o, 0xCE) += d;
        if (p5) { F16(p5, 0xBE) += d; F16(p5, 0xCE) += d; }
    }
}

static void wooden_branch_step(u8 *o, int two_links)
{
    u8 st = F8(o, ST);
    s16 w;
    switch (st) {
    case 0:
        if (F8(o, B69)) { F8(o, B6B) = F8(o, B69); S16(o, W20) = 0; F8(o, ST) = st + 1; }
        break;
    case 1:
        if (F8(o, B69) == 0) { F8(o, ST) = st + 1; break; }
        w = S16(o, W20);
        if (w < 32) {
            S16(o, W20) = w + 4;
            bend(o, P32(o, D90), two_links ? P32(o, D94) : 0, F8(o, B69), 1);
        }
        break;
    case 2:
        F8(o, ST) = st + 1;
        break;
    case 3:
        w = S16(o, W20);
        if (w == 0) { F8(o, ST) = 0; break; }
        S16(o, W20) = --w;
        if (w & 3) break;
        bend(o, P32(o, D90), two_links ? P32(o, D94) : 0, F8(o, B6B), -1);
        break;
    }
}

void func_80132BB8(void *o)
{
    wooden_branch_step(o, 1);
}

void func_80132DD0(void *o)
{
    wooden_branch_step(o, 0);
}

/* func_8011DF5C (the second global function of chainedPartPlaceChild.c, a chained-object placement handler of
 * updateA00L4Type06: T7468[0] = data_80138B98[0] = 0x8011DF60) lives at 0x8011DF60: its C name is 4 bytes short and
 * the matching build PROVIDEs it only as func_8011DF5C__8011DF60.  gen_port.py reads that renamed PROVIDE, but a
 * build whose build/full lacks it (the real-Windows build of 652d9dc still printed "32 function ... 2308 code
 * labels kept" and jumped to the retail address) needs a definition named by the address: gen2's AddrMap takes
 * every func_XXXXXXXX of port2/runtime as the native code of that address, so the word is relocated here whatever
 * the PROVIDE names say. */
void func_8011DF60(void *o)
{
    extern void func_8011DF5C(void *);
    func_8011DF5C(o);
}
