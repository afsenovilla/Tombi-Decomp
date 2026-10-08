// FUNC 8011ad78 356 X014
// MATCHING 8011ad78 356
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVEC;
extern char D_1F800000[];
extern char DAT_1f800000[]; /* same scratchpad matrix, second name: the game reloads its address */
extern void FUN_80021f5c(void *);
extern void *MulMatrix0(void *, void *, void *);
extern void ApplyRotMatrix(SVEC *, void *);
extern int FUN_800202b4(TObj *);
extern void ObjFreeDup(TObj *);

#define V5C (*(int *)((char *)o + 0x5c))
#define V60 (o->d60)
#define V64 (o->d64)

void func_8011AD78(TObj *o)
{
    TObj *e;
    SVEC v;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0a = 0x10;
        o->da0 = 0;
        o->box0 = 6;
        o->box1 = 0xc;
        o->box2 = 6;
        o->box3 = 0xc;
        break;
    case 1:
        e = (TObj *)o->d90;
        FUN_80021f5c(D_1F800000);
        v.vx = o->d30 >> 16;
        v.vy = o->d34 >> 16;
        v.vz = o->d38 >> 16;
        MulMatrix0(&e->w48, DAT_1f800000, &o->w48);
        ApplyRotMatrix(&v, &o->w5c);
        V5C += e->a.p.whole;
        V60 += e->y.p.whole;
        V64 += e->b.p.whole;
        o->a.p.whole = V5C;
        o->y.p.whole = V60;
        o->b.p.whole = V64;
        FUN_800202b4(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
