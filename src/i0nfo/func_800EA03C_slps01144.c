// VERSION SLPS_01144 info 800EA03C 576
// JP version of PAL i0nfo 800e8644 (overheadPlayerInit.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern void func_80020DE4(TObj *o);
extern int func_8005B930(void);
extern void overheadSpawnPlayerShadow(TObj *o, int a);
extern void overheadSpawnFollower(TObj *o);
extern void *D_80010760;
extern char D_8001168C[];
typedef struct { unsigned short a, b; } Pair7A;
extern Pair7A D_80076B84[];
extern Pair7A D_80076B86[];
extern TObj *D_8009DEE0;
extern unsigned char D_8009E518;
typedef struct { unsigned char c; } C970S; typedef struct { int v; } SPV;
extern unsigned short D_8009E52A[];
extern unsigned char D_8009E538[];
extern unsigned char D_8009EA53;
extern unsigned char D_8009EAAE;
extern signed char D_8009EE58;
extern unsigned char D_8009EE5B[];
extern unsigned short D_800A7B80[];
extern unsigned char D_800A7BE3;
extern unsigned char D_800A7C3A[];
extern unsigned short D_800A7C3C[];

void func_800EA03C(TObj *o)
{
    TObj *p;
    void *an;
    int pad[2];
    D_8009DEE0 = (TObj *)((char *)o + 0xE8);
    if (o->step == 0) {
        o->anim = D_8001168C;
        func_80020DE4(o);
        p = D_8009DEE0;
        if (func_8005B930() != 1) {
            func_8005B930();
        }
        S16(p, 0x14) = 0;
        o->w1e = 0;
        D_800A7C3A[0] = D_8009EE5B[0];
        D_800A7B80[0] = D_80076B84[D_8009E538[0]].a;
        D_800A7C3C[0] = D_80076B86[D_8009EE5B[0]].a;
        U16(D_8009DEE0, 0x28) = 0xFFFF;
        U16(D_8009DEE0, 0x2A) = 0xFFFF;
        D_8009DEE0->animFrame = 0xFFFF;
        D_8009DEE0->active = 0;
        o->visible = 1;
        o->b0a = 1;
        o->ba5 = 0;
        o->wb0 = 0;
        o->velH = 0;
        o->velV = 0;
        o->wb2 = 0;
        o->b6b = 1;
        o->b9d = 0;
        if (D_8009E52A[0] != 0) {
            o->animFrame = 4;
        }
        if (D_8009EAAE != 0) {
            o->subtype = 1;
        }
        S16(o, 0x9A) = D_8009E518;
        o->w98 = ((C970S *)&D_8009E518)->c;
        *(signed char *)&o->b0f = -8;
        an = D_80010760;
        o->d3c = ((SPV *)0x1F8002F0)->v;
        o->anim = an;
        func_80020DE4(o);
        overheadSpawnPlayerShadow(o, 1);
        if (D_8009EA53 == 3) {
            overheadSpawnFollower(o);
        }
        D_800A7BE3 = 0;
        o->b04 = 1;
        o->step = 0x63;
        o->state = 0;
        switch (D_8009EE58) {
        case 4:
            o->h->p.whole += 8;
            o->y.p.whole += 8;
            break;
        case 3:
            o->h->p.whole = -0x49;
            o->animFrame = 6;
            o->b04 = 4;
            o->step = 5;
            o->state = 0;
            o->substep = 0;
            break;
        }
    }
}
#endif
