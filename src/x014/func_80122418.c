// FUNC 80122418 936 X014
// MATCHING 80122418 936
#include "TOBJ.H"

typedef struct { void *a[3]; } AT;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern AT D_80129FCC[];
extern int D_1F8002DC[];
extern int D_1F800334;
extern unsigned short D_8009C962;
extern unsigned char D_8009C942, D_8009C93F;
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern TObj *FUN_800184d8(void);
extern void FUN_80018790(TObj *);
extern void FUN_80021f5c(void *);
extern void RotMatrix(SVECTOR *, void *);
extern void FUN_80020078(TObj *, int);
extern void func_8012194C(TObj *, int);
extern void func_80121B4C(TObj *);
extern void func_80121D08(TObj *);
extern void func_80121F90(TObj *);

void func_80122418(TObj *o)
{
    unsigned char s = o->b04;
    TObj *e;

    switch (s) {
    case 0:
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            o->box0 = 10;
            o->box1 = 0x14;
            o->box2 = 10;
            o->box3 = 0x14;
            *(signed char *)&o->b0f = -9;
            o->b0a = 0;
            o->w1e = 1;
            o->b0c = 0;
            o->b0d = 0;
            o->d3c = D_1F8002DC[0];
            o->wac = 0;
            o->anim = D_80129FCC[o->subtype].a[0];
            AnimLoadDuration(o);
            break;
        case 3: {
            int *pp = &D_1F800334;
            int *q = (int *)*pp;
            q += 1;
            if (D_8009C962 == 7) o->da0 = *pp + q[4];
            else o->da0 = *pp + q[1];
            o->active = 2;
            o->ba4 = 1;
            o->box0 = 0x50;
            o->box1 = 0xa0;
            o->box2 = 0x50;
            o->box3 = 0xa0;
            o->b0a = 0x11;
            o->b0c = 0;
            func_8012194C(o, 0);
            e = FUN_800184d8();
            if (e) {
                e->type = 0x42;
                e->d30 = 0x150000;
                e->d34 = -0x40000;
                e->active = 1;
                e->subtype = 0;
                e->d38 = -0x5a0000;
                e->h->raw = o->h->raw + e->d30;
                e->y.raw = o->y.raw + e->d34;
                e->d->raw = o->d->raw + e->d38;
                e->d90 = (int)o;
            }
            *(signed char *)&o->b0f = 0;
            break;
        }
        case 4: {
            int *pp = &D_1F800334;
            int *q = (int *)*pp;
            q += 1;
            if (D_8009C962 == 7) o->da0 = *pp + q[3];
            else o->da0 = *pp + q[0];
            o->active = 2;
            o->ba4 = 1;
            o->box0 = 0x38;
            o->box1 = 0x70;
            o->box2 = 0x38;
            o->box3 = 0x70;
            o->b0a = 0x11;
            o->b0c = 0;
            func_8012194C(o, 1);
            *(signed char *)&o->b0f = 0;
            break;
        }
        }
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b04++;
        break;
    case 1:
        if (D_8009C942 == 1) {
            ObjCullRegister(o);
            break;
        }
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            func_80121B4C(o);
            ObjCullRegister(o);
            break;
        case 3:
        case 4: {
            SVECTOR v;
            void *m = &o->w48;
            FUN_80021f5c(m);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, m);
            FUN_80020078(o, o->box0);
            if (D_8009C93F != 1) func_80121F90(o);
            break;
        }
        }
        break;
    case 2:
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            func_80121D08(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
