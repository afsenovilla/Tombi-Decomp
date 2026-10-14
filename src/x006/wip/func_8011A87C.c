// FUNC 8011a87c 1644 X006
// FLAGS -O2 -G0 -fno-expensive-optimizations
#include "gvars.h"
/* score 179 (was 276): -fno-expensive-optimizations (the game reloads lui/addiu D_1F800000 at every use, like
   matched x014 func_80119344) + o pinned to $17 + a separate temp f for the ObjAlloc result (game $3).
   Remaining: the pin leaves `move $7,$4` in the prologue (one extra insn, $7 used for one sb); case-0 list walker e
   gets $5 (game $4); k=1 inner step e lands in $5 (game $2) and a missing nop at its first branch; case-2 a0/a1
   swap. Earlier notes: RotMatrixY((unsigned short)p->wb4), empty `case 2:` for the k=0 step switch, D_8009C962_b
   alias for the case-1/k=1 re-read. */
#include "TOBJ.H"

typedef struct { short vx, vy, vz, pad; } SVECTOR;
#define M(o) ((void *)((char *)(o) + 0x48))
#define V5C(o) (*(int *)((char *)(o) + 0x5c))
#define W32(o) ((o)->d30 >> 16)
#define W36(o) ((o)->d34 >> 16)
#define W3A(o) ((o)->d38 >> 16)

extern char D_1F800000[];
extern unsigned short D_8009C962, D_8009C982;
extern unsigned short D_8009C962_b[] asm("D_8009C962");
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
extern void func_801198F0(TObj *);
extern void func_8011A73C(TObj *);
extern void func_8011AF84(TObj *);
extern int func_8011B238(TObj *);
extern void func_80118304(int, int, int);

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

void func_8011A87C(TObj *o_)
{
    register TObj *o asm("$17") = o_;
    unsigned char s = o->b04;
    SVECTOR v;
    TObj *e, *p;
    void *m;

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
            for (e = (TObj *)o->d94; e; e = (TObj *)e->d94) {
                e->d90 = (int)o;
                e->d30 = e->a.raw - o->a.raw;
                e->d34 = e->y.raw - o->y.raw;
                e->d38 = e->b.raw - o->b.raw;
            }
            m = M(o);
            o->b0a = 0x15;
            o->da0 = 0;
            getBaseMatrix(m);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, m);
            if (D_8009C962 == 0) {
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
            if (D_8009C962 == 0) {
                func_801198F0(o);
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
            if (D_8009C962 == 0) {
                func_8011A73C(o);
            } else {
                unsigned char t = o->step;
                e = (TObj *)o->d90;
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
                    if (e->wb6 == 0) o->step = t + 1;
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
            e = (*(TObj **)&GVARS.p50);
            switch (t) {
            case 0:
                o->step = t + 1;
                e->anim = D_801229A4[0];
                AnimLoadDuration(e);
                func_80118304(6, 0x64, 0x64);
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
