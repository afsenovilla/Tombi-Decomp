// FUNC 801276cc 336 X009
/* score 29 (n71). Same two diffs as before (case 0's `li v1,300` placement / `substep=3` path jumping into
   case 1's `sb; j end [move v0,0]`, and the bne keeping a nop). Findings this round:
   - `o->timer = 300;` written directly after the D_8009CDC9 test (no k local) already gives the game's case-0
     block order (li v1,300 first, move a1,zero before sh): the old k/boost theory is moot.
   - The game's `bne ... [nop]` + `L_b4: j end [move v0,0]` shape = reorg could not steal `move v0,0` because a
     USE marker of it sat in the fallthrough path (mark_target_live_regs treats regs set by a USE'd insn as live).
     That needs case 1's `return 0` to be its own `move; j end` block that jump2 does NOT cross-jump into case 0's
     post-call `move v0,0; j end` (the label before case 1's move lowers find_cross_jump's minimum to 1).
   - Variant P (case 0 post-call `break` + `return 0;` after the switch inside the if-block, other cases
     `return 0`) reproduces case 0 -> `j L15 [li v0,3]` into case 1's sb and the post-call `j end [move]`, but then
     case 1's tail merges with the after-switch block (score 38, 2 nops short). Variants tried: break/return mixes
     for cases 1/2 (40-63), `if (--timer != -1) return 0;` two-return forms (46-60). */
#include "TOBJ.H"
extern unsigned char D_8009CDC9;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93E;
extern void FUN_8005a9a4(int, int);

int func_801276CC(TObj *o)
{
    unsigned char t;
    if (o->subtype == 0) {
        switch (o->substep) {
        case 0:
            if (D_8009CDC9 == 0xff) {
                o->substep = 3;
                return 0;
            }
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_8009C93E = 1;
            o->timer = 300;
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
