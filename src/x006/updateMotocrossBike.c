// FUNC 8011a87c 1644 X006
// MATCHING 8011a87c 1644
// FLAGS -O2 -G0 -fno-expensive-optimizations
#include "gvars.h"
#include "TOBJ.H"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
#define M(o) ((void *)((char *)(o) + 0x48))
#define V5C(o) (*(int *)((char *)(o) + 0x5c))
#define W32(o) ((o)->d30 >> 16)
#define W36(o) ((o)->d34 >> 16)
#define W3A(o) ((o)->d38 >> 16)

extern char D_1F800000[];
extern unsigned short GAME_SELECTED_SECTION, D_8009C982;
extern unsigned short D_8009C962_b[] asm("GAME_SELECTED_SECTION");
extern void *D_801229A4[];
extern TObj *ObjAlloc(void);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void AnimLoadDuration(TObj *);
extern void getBaseMatrix(void *);
extern void RotMatrix(SVECTOR *, void *);
extern void RotMatrixY(int, void *);
extern void RotMatrixZ(int, void *);
extern void MulMatrix0(void *, void *, void *);
extern void ApplyRotMatrix(SVECTOR *, void *);
extern void motocrossBikeRide(TObj *);
extern void motocrossBikeBob(TObj *);
extern void func_8011AF84(TObj *);
extern int func_8011B238(TObj *);
extern void spawnRaceMessage(int, int, int);

#define TAIL { \
    v.vx = W32(o); \
    v.vy = W36(o); \
    v.vz = W3A(o); \
    MulMatrix0(M(p), D_1F800000, M(o)); \
    ApplyRotMatrix(&v, &V5C(o)); \
    V5C(o) += p->a.p.whole; \
    o->d60 += p->y.p.whole; \
    o->d64 += p->b.p.whole; \
    o->a.p.whole = V5C(o); \
    o->y.p.whole = o->d60; \
    o->b.p.whole = o->d64; \
}

void updateMotocrossBike(TObj *o_)
{
    register TObj *o asm("$17");
    unsigned char s;
    SVECTOR v;
    TObj *e, *p;
    TObj *e1;
    TObj *e2;
    register TObj *e3 asm("$4");
    void *m;

    o = o_; asm("" : "=r"(o) : "0"(o)); s = o->b04;
    switch (s) {
    case 0: {
        int x = o->a.p.whole, y = o->y.p.whole, z = o->b.p.whole;
        int k = o->b0c;
        o->b04 = s + 1;
        o->active = 2;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        V5C(o) = x;
        o->d60 = y;
        o->d64 = z;
        switch (k) {
        case 0:
            for (e1 = (TObj *)o->d94; e1; e1 = (TObj *)e1->d94) {
                e1->d90 = (int)o;
                e1->d30 = e1->a.raw - o->a.raw;
                e1->d34 = e1->y.raw - o->y.raw;
                e1->d38 = e1->b.raw - o->b.raw;
            }
            m = M(o);
            o->b0a = 0x15;
            o->da0 = 0;
            getBaseMatrix(m);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, m);
            if (GAME_SELECTED_SECTION == 0) {
                TObj *f = ObjAlloc();
                if (f) {
                    f->active = 1;
                    f->type = 0x3b;
                    f->d30 = -4;
                    f->d34 = -0x16;
                    f->d38 = 0;
                    f->d90 = (int)o;
                }
                (*(TObj **)&GVARS.p50) = f;
            }
            (*(TObj **)&GVARS.p4c) = o;
            break;
        case 1:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            o->ba4 = 0;
            getBaseMatrix(D_1F800000);
            TAIL;
            break;
        case 2:
        case 3:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            getBaseMatrix(D_1F800000);
            RotMatrixZ(p->velH, D_1F800000);
            RotMatrixY((unsigned short)p->wb4, D_1F800000);
            TAIL;
            break;
        case 4:
        case 5:
            p = (TObj *)o->d90;
            o->b0a = 0x15;
            getBaseMatrix(D_1F800000);
            RotMatrixZ(p->velH, D_1F800000);
            TAIL;
            break;
        }
        break;
    }
    case 1:
        ObjCullRegister(o);
        switch (o->b0c) {
        case 0:
            if (GAME_SELECTED_SECTION == 0) {
                motocrossBikeRide(o);
            } else {
                unsigned char t = o->step;
                switch (t) {
                case 0:
                    o->step = t + 1;
                    o->wb6 = 0x200;
                    o->wb4 = 0;
                    o->velH = 0;
                    break;
                case 1:
                    o->wb6 -= 8;
                    if (o->wb6 < 0) {
                        o->wb6 = 0;
                        o->step++;
                    }
                    o->velX = o->wb6 >> 1;
                    o->a.raw += o->wb6 << 8;
                    break;
                    break;
                case 2:
                    break;
                }
                o->velH += o->velX;
            }
            m = M(o);
            getBaseMatrix(m);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, m);
            break;
        case 1:
            if (GAME_SELECTED_SECTION == 0) {
                motocrossBikeBob(o);
            } else {
                unsigned char t = o->step;
                e2 = (TObj *)o->d90;
                switch (t) {
                case 0:
                    if (D_8009C962_b[0] == 1 && D_8009C982) {
                        o->wac = 3;
                        func_8011AF84(o);
                        o->step = 3;
                    } else {
                        o->wac = 1;
                        o->step++;
                    }
                    break;
                case 1:
                    if (e2->wb6 == 0) o->step = t + 1;
                    break;
                case 2:
                    if (func_8011B238(o) == t) o->step++;
                    break;
                }
            }
            p = (TObj *)o->d90;
            getBaseMatrix(D_1F800000);
            TAIL;
            break;
        case 2:
        case 3:
            p = (TObj *)o->d90;
            getBaseMatrix(D_1F800000);
            RotMatrixZ(p->velH, D_1F800000);
            RotMatrixY((unsigned short)p->wb4, D_1F800000);
            TAIL;
            break;
        case 4:
        case 5:
            p = (TObj *)o->d90;
            getBaseMatrix(D_1F800000);
            RotMatrixZ(p->velH, D_1F800000);
            TAIL;
            break;
        }
        break;
    case 2:
        ObjCullRegister(o);
        if (o->b0c == 0) {
            unsigned char t = o->step;
            e3 = (*(TObj **)&GVARS.p50);
            switch (t) {
            case 0:
                o->step = t + 1;
                e3->anim = D_801229A4[0];
                AnimLoadDuration(e3);
                spawnRaceMessage(6, 0x64, 0x64);
                break;
            case 1:
                break;
            case 2:
                *(short *)((char *)o + 0xd0) = 0;
                break;
            }
        }
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
