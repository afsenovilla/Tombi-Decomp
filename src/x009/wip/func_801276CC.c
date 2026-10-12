// FUNC 801276cc 336 X009
/* score 29: game fills the case-0 bne delay slot with li v1,300 (here k lands in a1), and case 1's false branch goes to its own j end; move v0,zero block (here reorg threads it to the epilogue with move v0,zero in the bne slot).
   (o32): jump2 dump: case 1's return-0 is cross-jumped into case 0's post-call return already in jump2; game keeps both.
   o->timer = 300 without k gives v1 but reorg then takes li v0,1 for the slot (29). Tried: short/uchar return, r variable
   with one return (47), inverted ifs, break + return after switch.
   n60: with `k = 300;` moved after the if (or plain `o->timer = 300`) k gets v1 like the game, but sched2 puts
   `li v0,1` first in the fallthrough block (reorg takes the first insn): sched1 schedules `li k` at T-9 right
   before the sh (its only consumer, placed at T-8 because the lbu is latency-blocked) while `li 1` is last
   (T-14), so k gets the higher luid. The game needs `li k` scheduled after `li 1` in sched1 (k ready only at
   T-14). Un-boosting k (k = -1 for the case 1 compare, k = 1 for case 3, k = 0 for the call arg) does not
   delay it (still the only ready insn at T-9). Statement orders of {globals, timer, substep++} x k forms: 29/45+.
   Second diff: jump2 cross-jumps case 1's `set v0 0; jump end` into case 0's post-call return (candidate 82
   in jump_chain, 1 insn + label = minimum 1); the game keeps both copies and so its bne targets `j end` with
   move v0,zero in the j's slot (steal_delay_list_from_target then does not fire). Case-1 forms A..I tried. */
#include "TOBJ.H"
extern unsigned char D_8009CDC9;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93E;
extern void FUN_8005a9a4(int, int);

int func_801276CC(TObj *o)
{
    unsigned char t;
    int k;
    if (o->subtype == 0) {
        switch (o->substep) {
        case 0:
            k = 300;
            if (D_8009CDC9 == 0xff) {
                o->substep = 3;
                return 0;
            }
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_8009C93E = 1;
            o->timer = k;
            o->substep++;
            FUN_8005a9a4(0x25, 0);
            return 0;
        case 1:
            if (--o->timer == -1) o->substep++;
            return 0;
        case 2:
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            o->substep++;
            return 0;
        case 3:
            return 1;
        }
    } else {
        t = (*(TObj **)&o->wa8)->substep;
        o->substep = t;
        return t == 3;
    }
}
