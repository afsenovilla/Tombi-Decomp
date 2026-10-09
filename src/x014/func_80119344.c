// FUNC 80119344 716 X014
// MATCHING 80119344 716
// FLAGS -O2 -G0 -fno-expensive-optimizations
#include "TOBJ.H"

typedef struct { short m[3][3]; int t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
#define M(o) ((MATRIX *)((char *)(o) + 0x48))
extern MATRIX D_1F800000;
extern TObj *volatile D_1F800334;
extern signed char D_80125C38[][2];
extern TObj *FUN_800184d8(void);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void func_801190D0(TObj *);
extern void func_8011920C(TObj *);
extern void FUN_80021f5c(MATRIX *);
extern void RotMatrix(SVECTOR *, MATRIX *);
extern void RotMatrixX(int, MATRIX *);
extern void RotMatrixZ(int, MATRIX *);
extern void MulMatrix0(MATRIX *, MATRIX *, MATRIX *);
extern void ApplyRotMatrix(SVECTOR *, int *);

void func_80119344(TObj *o)
{
    SVECTOR sv;
    TObj *e;
    TObj *p;
    short i;
    TObj *volatile *q;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 2;
        if (o->subtype != 0)
            break;
        i = 0;
        q = &D_1F800334;
        for (; i < 4; i++) {
            e = FUN_800184d8();
            if (e == 0)
                continue;
            e->active = 2;
            e->type = 0x2a;
            e->subtype = 1;
            e->b0c = i;
            e->b0a = 0x15;
            e->d30 = D_80125C38[i][0] << 16;
            e->d34 = D_80125C38[i][1] << 16;
            e->d38 = 0;
            e->h->raw = o->h->raw + e->d30;
            e->y.raw = o->y.raw + e->d34;
            e->d->raw = o->d->raw + e->d38;
            {
                TObj *t = *q;
                int b = (int)*q;
                int d = t->d30;
                e->ba4 = 0;
                e->d90 = (int)o;
                e->d94 = 0;
                e->da0 = b + d;
            }
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->subtype == 0) {
            func_801190D0(o);
            FUN_80021f5c(M(o));
            sv.vx = o->d84;
            sv.vy = o->d88;
            sv.vz = o->d8c;
            RotMatrix(&sv, M(o));
        } else {
            func_8011920C(o);
            p = (TObj *)o->d90;
            FUN_80021f5c(&D_1F800000);
            RotMatrixX(o->d84, &D_1F800000);
            RotMatrixZ(o->d8c, &D_1F800000);
            sv.vx = o->d30 >> 16;
            sv.vy = o->d34 >> 16;
            sv.vz = o->d38 >> 16;
            MulMatrix0(M(p), &D_1F800000, M(o));
            ApplyRotMatrix(&sv, M(o)->t);
            M(o)->t[0] += p->a.p.whole;
            M(o)->t[1] += p->y.p.whole;
            M(o)->t[2] += p->b.p.whole;
            o->a.p.whole = M(o)->t[0];
            o->y.p.whole = M(o)->t[1];
            o->b.p.whole = M(o)->t[2];
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
