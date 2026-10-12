/* score 29 (two diffs left, see below). n67 findings from the cc1 -dS/-dR traces (sched1/sched2 are both
   BACKWARD list schedulers: picked first = placed last; ties: boosted (reg set once) first, else store beats ALU,
   else higher luid first):
   (1) case 0 `li v1,300` must be the FIRST insn of the fallthrough block (reorg then puts it in the bne slot):
   with k multi-set (k = -1 for the case-1 compare, so `li k` is not birthing-boosted) and the three globals as
   fields of one struct (so the lbu of substep depends on the sb's), the only thing left is that `move a1,zero`
   (call arg) is boosted and leaves at T-4: at T-9 `li k` is alone in the ready list. If a1 is set twice in the
   function (test: a second two-arg call in case 3) case 0 comes out exactly like the game (li 300; li 1; sb;sb;sb;
   lbu; move a1; sh; addiu; sb) -- but the game has only one call, so what un-boosts a1 is still unknown.
   (2) case 1: jump2 cross-jumps case 1's `move v0,0; j end` into case 0's post-call return (1 matching insn + the
   bne's target label before the move lowers the minimum to 1); the game keeps both copies. Note reorg itself
   never steals `move v0,0` into the bne slot (a load in the fallthrough is not eligible and the fallthrough is
   tried first), so a plain `if (--o->timer == -1) o->substep++; return 0;` layout is right once the cross-jump
   is prevented (the label right before the move is the problem; every if/else/goto form tried gives it). */
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
