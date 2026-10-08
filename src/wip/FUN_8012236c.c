// FUNC 8012236c 340 X000
/* score 22 (size 340: splat fragment 80122410 is the tail of case 0). case 1 fixed except lw 0x30 vs DAT_186 lui order (DAT_1f800186 as [0]). case 0: game order looks like w1e,b0d,anim,d3c,b0f,b04,d34,d30 (11 hoisted into switch delay slot; that order gives 53 because lh y gets hoisted into the anim load slot); best found by random statement permutation = 22. Tried raw-offset stores, scalar/[0] D_1F8002D4, b04++ vs b04=b04+1. */
#include "TOBJ.H"
extern short DAT_1f800176;
extern unsigned short DAT_1f800186[];
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
        o->d34 = o->y.p.whole;
        o->anim = PTR_8013b1fc[o->subtype];
        o->b0d = 0;
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->w1e = 11;
        o->b04 = o->b04 + 1;
        break;
    case 1:
        r = DAT_1f800176;
        k = r;
        if (r < 0x71a) {
            j = DAT_1f800186[0];
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
