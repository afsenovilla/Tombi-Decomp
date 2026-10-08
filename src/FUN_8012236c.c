// FUNC 8012236c 340 X000
// MATCHING 8012236c 340
/* size 340: splat fragment 80122410 is the tail of case 0. */
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
    int x;
    switch (o->b04) {
    case 0:
        o->w1e = 11;
        o->b0d = 0;
        o->anim = PTR_8013b1fc[o->subtype];
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        r = DAT_1f800176;
        k = r;
        if (r < 0x71a) {
            x = o->d30;
            j = DAT_1f800186[0];
            o->b.p.whole = 0;
            o->a.p.whole = (short)(x - k) >> 3;
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
