// FUNC 80120d04 1056 X000
// MATCHING 80120d04 1056
// FLAGS -O2 -G0 -fno-expensive-optimizations
#include "TOBJ.H"
#include "gamestate.h"

extern char D_1F800000[];
void getBaseMatrix(void *m);
void RotMatrixX(int r, void *m);
void RotMatrixY(int r, void *m);
void RotMatrix(short *v, void *m);
void MulMatrix0(void *a, void *b, void *c);
void ApplyRotMatrix(short *v, void *out);
void ObjCullRegister(TObj *o);
void ObjFreeDup(TObj *o);
void l4Type1BInitModel(TObj *o);
void l4Type1BCarryPlayer(TObj *o);
void l4Type1BProbe(TObj *o);

#define C0() \
    getBaseMatrix(&o->w48); \
    sv[0] = o->d84; \
    sv[1] = o->d88; \
    sv[2] = o->d8c; \
    RotMatrix(sv, &o->w48);

#define VEC() \
    sv[0] = o->d30 >> 16; \
    sv[1] = o->d34 >> 16; \
    sv[2] = o->d38 >> 16; \
    MulMatrix0(&p->w48, D_1F800000, &o->w48); \
    ApplyRotMatrix(sv, &o->w5c); \
    *(int *)&o->w5c += p->a.p.whole; \
    o->d60 += p->y.p.whole; \
    o->d64 += p->b.p.whole; \
    o->a.p.whole = *(int *)&o->w5c; \
    o->y.p.whole = o->d60; \
    o->b.p.whole = o->d64;

void l4Type1BArea0Update(TObj *o)
{
    short sv[3];
    TObj *p;
    TObj *e;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        if (((unsigned short*)&GAME.selectedSection)[0] == 0)
            return;
        o->active = 2;
        *(int *)&o->w5c = o->a.p.whole;
        o->d60 = o->y.p.whole;
        o->d64 = o->b.p.whole;
        switch (o->b0c) {
        case 0:
            for (e = (TObj *)o->d94; e != 0; e = (TObj *)e->d94) {
                e->d90 = (int)o;
                e->d30 = e->a.raw - o->a.raw;
                e->d34 = e->y.raw - o->y.raw;
                e->d38 = e->b.raw - o->b.raw;
            }
            o->b0a = 0x15;
            o->da0 = 0;
            C0();
            break;
        case 1:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            o->ba4 = 0;
            getBaseMatrix(D_1F800000);
            VEC();
            break;
        case 2:
        case 3:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            getBaseMatrix(D_1F800000);
            RotMatrixX(p->velH, D_1F800000);
            RotMatrixY((unsigned short)p->wb4, D_1F800000);
            VEC();
            break;
        case 4:
        case 5:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            getBaseMatrix(D_1F800000);
            RotMatrixX(p->velH, D_1F800000);
            VEC();
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (((unsigned short*)&GAME.selectedSection)[0] == 0)
            return;
        switch (o->step) {
        case 0:
            l4Type1BInitModel(o);
            break;
        case 1:
            l4Type1BCarryPlayer(o);
            break;
        case 2:
            o->b0a = 0x15;
            switch (o->b0c) {
            case 0:
                l4Type1BProbe(o);
                C0();
                break;
            case 1:
                p = (TObj *)o->d90;
                getBaseMatrix(D_1F800000);
                VEC();
                break;
            case 2:
            case 3:
                p = (TObj *)o->d90;
                getBaseMatrix(D_1F800000);
                RotMatrixX(p->velH, D_1F800000);
                RotMatrixY((unsigned short)p->wb4, D_1F800000);
                VEC();
                break;
            case 4:
            case 5:
                p = (TObj *)o->d90;
                getBaseMatrix(D_1F800000);
                RotMatrixX(p->velH, D_1F800000);
                VEC();
                break;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
