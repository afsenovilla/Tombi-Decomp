// FUNC 8011921c 636 X011
// MATCHING 8011921c 636
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009D2B0;
extern unsigned char D_8009CFDF, D_8009CFF7;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8004d620(int, int);

void func_8011921C(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009D2B0 = 2;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        *(int *)((char *)o + 0x90) = FUN_8002dcc8(3, 0x15, &v);
        if (D_8009CFDF == 0) {
            D_8009CFDF = 1;
            D_8009CFF7++;
        }
        FUN_8004d620(D_8009CFF7 + 0x32, 2);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        D_800A60EA[0] = 0;
        o->timer = 4;
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer != 0) o->state = 9;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
