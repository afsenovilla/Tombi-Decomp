// VERSION SCUS_94236 x11 80116F84 616
// FLAGS -O2 -G0
#if defined(VERSION_SCUS_94236)
// USA version of PAL x011 bonsugeeResearcherTalk (src/x011/bonsugeeResearcherTalk.c): US globals.
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80118F14[];
extern unsigned char D_8009BCAA[];
extern signed char D_8009C618[];
extern unsigned char D_800A539C[], D_800A539D[], D_800A539E[];
extern short D_800A544A[];
extern short D_1F8001C6;
extern void func_800229FC(TObj *);
extern int func_800307AC(int, int, V6 *);
extern void func_8001E220(int, int, int);

void func_80116F84(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009BCAA[0] = 1;
        D_8009C618[0] = 2;
        D_800A539C[0] = 5;
        D_800A539D[0] = 0;
        D_800A539E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_80118F14[o->subtype].anims[2];
        func_800229FC(o);
        o->d90 = func_800307AC(2, 0, &v);
        o->state++;
        break;
    case 1: {
        TObj *q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->d90 = func_800307AC(2, 1, &v);
        o->state++;
        break;
    }
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        func_8001E220(0x41, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 3:
        if (--o->timer != 0) break;
        o->d90 = func_800307AC(2, 2, &v);
        o->state++;
        break;
    case 4: {
        TObj *q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->anim = D_80118F14[o->subtype].anims[0];
        func_800229FC(o);
        o->state++;
        break;
    }
    case 5:
        D_8009BCAA[0] = 0;
        D_800A544A[0] = 0;
        o->animFrame = o->wbc;
        D_1F8001C6 = 0;
        D_800A539C[0] = 1;
        D_800A539D[0] = 0;
        D_800A539E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
#endif
