// FUNC 8012171c 340 X000
// MATCHING 8012171c 340
#include "TOBJ.H"
extern char DAT_80138d6c[];
extern void FUN_8003ecb0(void *, void *, int, int, TObj *);

void FUN_8012171c(TObj *o)
{
    switch (o->step) {
    case 0:
        o->wb4 = 0x3c;
        o->step++;
        break;
    case 1:
        if (--*(unsigned short *)&o->wb4 == 0xffff)
            o->step++;
        break;
    case 2:
        o->wb4 = 0x78;
        o->step++;
        break;
    case 3:
        if (o->b0c == 0) {
            if (*(unsigned short *)&o->wb4 == 0x78)
                FUN_8003ecb0(DAT_80138d6c, &o->a, 0x20, -0x400, o);
            else if (*(unsigned short *)&o->wb4 == 0x5a)
                FUN_8003ecb0(DAT_80138d6c, &o->a, -0x20, -0x400, o);
        }
        if (--*(unsigned short *)&o->wb4 == 0xffff) {
            o->wb4 = 0x5a;
            o->step++;
        }
        break;
    case 4:
        if (!((*(unsigned short *)&o->wb4 >> 1) & 1))
            o->visible = 0;
        if (--*(unsigned short *)&o->wb4 == 0xffff)
            o->b04 = 3;
        break;
    }
}
