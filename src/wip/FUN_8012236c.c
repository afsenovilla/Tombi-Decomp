// FUNC 8012236c 312 X000
/* score 65: o=a2 y s=a0 ya coinciden; falta orden del case 0 y k/j del case 1 (k debe ir en a3, DAT_186 en a1). */
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
    int k, t;
    unsigned short j;
    char pad[8];
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
        k = DAT_1f800176;
        if (k < 0x71a) {
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
