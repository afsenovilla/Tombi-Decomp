// VERSION SCUS_94236 x11 801171EC 544
// FLAGS -O2 -G0
#if defined(VERSION_SCUS_94236)
// USA version of PAL x011 bonsugeeResearcherRepeatTalk (src/x011/bonsugeeResearcherRepeatTalk.c): US globals.
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

void func_801171EC(TObj *o)
{
    V6 v;
    TObj *p;
    TObj *q;

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
        o->d90 = func_800307AC(2, 1, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->d90 = func_800307AC(2, 2, &v);
        o->state++;
        break;
    case 2:
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->anim = D_80118F14[o->subtype].anims[0];
        func_800229FC(o);
        o->state++;
        break;
    case 3:
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
