// FUNC 801276cc 336 X009
// MATCHING 801276cc 336
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
extern void setEventComplete(int, int);

/* The `goto ret0` after the call keeps case 1's `return 0` block alive: with a plain `return 0` jump2
   cross-jumps case 1's `move v0,0; j end` into case 0's copy (the label before it drops the minimum to 1)
   and reorg then steals the move into the bne's delay slot. */
int checkMonsterHuntComplete(TObj *o)
{
    unsigned char t;
    if (o->subtype == 0) {
        switch (o->substep) {
        case 0:
            if (GCD.bcdc9 == 0xff) {
                o->substep = 3;
                return 0;
            }
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GVARS.b3e = 1;
            o->timer = 300;
            o->substep++;
            setEventComplete(0x25, 0);
            goto ret0;
        case 1:
            if (--o->timer == -1) o->substep++;
        ret0:
            return 0;
        case 2:
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
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
