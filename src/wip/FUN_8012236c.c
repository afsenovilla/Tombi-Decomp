// FUNC 8012236c 312 X000
/* score 64: short r + unsigned short k copy gives the game's lh/move/slti (as in func_80122724). Left: case 0 table load reg (game v0, sw anim right after), DAT_186 load should come before lw d34 (k then lands in a3). */
#include "TOBJ.H"
extern short DAT_1f800176;
extern unsigned short DAT_1f800186;
extern int DAT_800a4570[];
extern int DAT_1f8002d4[];
extern void *PTR_8013b1fc[];
extern void FUN_80018da4(TObj *);
extern void FUN_80018934(TObj *);

void FUN_8012236c(TObj *o)
{
    unsigned short k; int t;
    unsigned short j;
    short r;
    switch (o->b04) {
    case 0:
        o->w1e = 11;
        o->b0d = 0;
        o->anim = PTR_8013b1fc[o->subtype];
        o->b0f = 3;
        o->d3c = DAT_1f8002d4[0];
        o->b04++;
        o->d34 = o->y.p.whole;
        o->d30 = o->h->p.whole;
        break;
    case 1:
        r = DAT_1f800176;
        k = r;
        if (r < 0x71a) {
            j = DAT_1f800186;
            o->b.p.whole = 0;
            o->a.p.whole = (short)(o->d30 - k) >> 3;
            o->y.p.whole = (short)(o->d34 - j) >> 1;
            t = DAT_800a4570[0];
            *(unsigned char *)&o->visible = 1;
            o->y.p.whole -= (t >> 8) * 4;
            FUN_80018da4(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
