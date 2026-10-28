// FUNC 80122418 936 X014
// MATCHING 80122418 936
// VERSION SLPS_01144 x14 8011F720 936
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x14/nonmatchings/x14", func_8011F720_slps01144);
#else
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { void *a[3]; } AT;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
extern AT D_80129FCC[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern TObj *allocObjectLayer4(void);
extern void freeObjectLayer2(TObj *);
extern void getBaseMatrix(void *);
extern void RotMatrix(SVECTOR *, void *);
extern void cullObjectToDrawList(TObj *, int);
extern void spawnFlowerForestPigShotPair(TObj *, int);
extern void pigShotProbeWallFloor(TObj *);
extern void flowerForestPigShotScatter(TObj *);
extern void flowerForestPigShotSlide(TObj *);

void updateFlowerForestPigShot(TObj *o)
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
            o->d3c = ((int*)&SPAD.i2dc)[0];
            o->wac = 0;
            o->anim = D_80129FCC[o->subtype].a[0];
            AnimLoadDuration(o);
            break;
        case 3: {
            int *pp = &SPAD.i334;
            int *q = (int *)*pp;
            q += 1;
            if (GAME.selectedSection == 7) o->da0 = *pp + q[4];
            else o->da0 = *pp + q[1];
            o->active = 2;
            o->ba4 = 1;
            o->box0 = 0x50;
            o->box1 = 0xa0;
            o->box2 = 0x50;
            o->box3 = 0xa0;
            o->b0a = 0x11;
            o->b0c = 0;
            spawnFlowerForestPigShotPair(o, 0);
            e = allocObjectLayer4();
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
            int *pp = &SPAD.i334;
            int *q = (int *)*pp;
            q += 1;
            if (GAME.selectedSection == 7) o->da0 = *pp + q[3];
            else o->da0 = *pp + q[0];
            o->active = 2;
            o->ba4 = 1;
            o->box0 = 0x38;
            o->box1 = 0x70;
            o->box2 = 0x38;
            o->box3 = 0x70;
            o->b0a = 0x11;
            o->b0c = 0;
            spawnFlowerForestPigShotPair(o, 1);
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
        if (GVARS.b42 == 1) {
            ObjCullRegister(o);
            break;
        }
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            pigShotProbeWallFloor(o);
            ObjCullRegister(o);
            break;
        case 3:
        case 4: {
            SVECTOR v;
            void *m = &o->w48;
            getBaseMatrix(m);
            v.vx = o->d84;
            v.vy = o->d88;
            v.vz = o->d8c;
            RotMatrix(&v, m);
            cullObjectToDrawList(o, o->box0);
            if (GVARS.b3f != 1) flowerForestPigShotSlide(o);
            break;
        }
        }
        break;
    case 2:
        switch (o->subtype) {
        case 0:
        case 1:
        case 2:
            flowerForestPigShotScatter(o);
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
