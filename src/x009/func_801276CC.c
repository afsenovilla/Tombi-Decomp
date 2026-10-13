// FUNC 801276cc 336 X009
// MATCHING 801276cc 336
#include "TOBJ.H"
extern unsigned char D_8009CDC9;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93E;
extern void FUN_8005a9a4(int, int);

/* The `goto ret0` after the call keeps case 1's `return 0` block alive: with a plain `return 0` jump2
   cross-jumps case 1's `move v0,0; j end` into case 0's copy (the label before it drops the minimum to 1)
   and reorg then steals the move into the bne's delay slot. */
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
            goto ret0;
        case 1:
            if (--o->timer == -1) o->substep++;
        ret0:
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
