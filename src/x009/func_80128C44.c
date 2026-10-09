// FUNC 80128c44 648 X009
// MATCHING 80128c44 648
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;

extern unsigned char D_8009C942[], D_8009D2B0, D_8009CDCC[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[], D_1F8001C6;
extern AT3 D_8012B2E4[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_8005a8a8(int, int, int);

void func_80128C44(TObj *o)
{
    B12 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->anim = D_8012B2E4[o->subtype].p[32];
        FUN_8001fe94(o, 0);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        if (D_8009CDCC[0] == 0)
            o->d90 = (int)FUN_8002dcc8(2, 6, &v);
        else
            o->d90 = (int)FUN_8002dcc8(2, 7, &v);
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_8012B2E4[o->subtype].p[24];
        FUN_8001fe6c(o);
        o->timer = 4;
        if (D_8009CDCC[0] == 0) {
            FUN_8005a8a8(0x28, 0, 0);
            o->timer = 200;
        }
        D_800A60EA[0] = 0;
        o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        if (--o->timer != 0)
            o->state = 9;
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
