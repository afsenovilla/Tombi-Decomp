// FUNC 8011759c 604 X009
// MATCHING 8011759c 604
#include "TOBJ.H"

typedef struct { short vx, vy, vz, pad; } SVEC;

extern TObj *D_8009C948, *D_8009C94C[];
extern unsigned char D_8009C964, D_8009C93A, D_8009C940, D_8009C941, D_8009D07E;
extern void FUN_800201ac(TObj *, int);
extern void FUN_80021f5c(void *);
extern void RotMatrix(SVEC *, void *);
extern void FUN_8001888c(TObj *);
extern void func_80117D48(TObj *);
extern void func_801177F8(TObj *);
extern void func_801178B0(TObj *);

void func_8011759C(TObj *o)
{
    TObj *e;
    SVEC v;

    switch (o->b04) {
    case 0:
        e = (TObj *)o->d94;
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b6a = 0;
        o->b6b = 0;
        o->ba4 = 0;
        *(int *)&o->w5c = o->a.p.whole;
        o->d60 = o->y.p.whole;
        o->d64 = o->b.p.whole;
        while (e) {
            *(TObj **)&e->wa8 = o;
            if ((e->category == 4 && (TObj *)e->d90 != o) || e->category == 3) {
                e->d90 = (int)o;
                e->d30 = e->a.raw - o->a.raw;
                e->d34 = e->y.raw - o->y.raw;
                e->d38 = e->b.raw - o->b.raw;
            }
            e = (TObj *)e->d94;
        }
        o->d30 = o->h->raw;
        o->d34 = o->y.raw;
        o->d38 = o->d->raw;
        if (o->animFrame == 2) {
            D_8009C948->d94 = (int)o;
            D_8009C94C[0]->d94 = (int)o;
        } else {
            func_80117D48(o);
        }
        break;
    case 1:
        if (D_8009C964 == 0x20 || D_8009C93A != 1) {
            FUN_800201ac(o, 0xa0);
            func_801177F8(o);
            func_801178B0(o);
            FUN_80021f5c(&o->w48);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, &o->w48);
            if (!o->subtype && D_8009C940 && D_8009C941 == 0x8a) D_8009D07E = 1;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_8001888c(o);
        break;
    }
}
