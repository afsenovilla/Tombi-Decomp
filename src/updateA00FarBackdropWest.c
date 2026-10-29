// FUNC 8012236c 340 X000
// MATCHING 8012236c 340
// VERSION SLPM_80165 x00 8011976C 340
// VERSION SLPS_01144 x00 8012135C 340
#if defined(VERSION_SLPM_80165) || defined(VERSION_SLPS_01144)
// Demo version of PAL x000 8012236C (FUN_8012236c.c): reads 0x1F80017A (PAL 0x1F800186).
/* size 340: splat fragment 80122410 is the tail of case 0. */
#include "TOBJ.H"
extern short SCREEN_ORIGIN_X;
extern unsigned short D_1F80017A[];
#define SCREEN_ORIGIN_Y D_1F80017A
extern int DAT_800a4570[];
extern int DAT_1f8002d4[];
extern void *A00_FAR_BACKDROP_WEST_ANIMS[];
extern void FUN_80018da4(TObj *);
extern void freeObjectLayer7(TObj *);

void FUN_8012236c(TObj *o)
{
    unsigned short k; int t;
    unsigned short j;
    short r;
    int x;
    switch (o->b04) {
    case 0:
        o->w1e = 11;
        o->b0d = 0;
        o->anim = A00_FAR_BACKDROP_WEST_ANIMS[o->subtype];
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        r = SCREEN_ORIGIN_X;
        k = r;
        if (r < 0x71a) {
            x = o->d30;
            j = SCREEN_ORIGIN_Y[0];
            o->b.p.whole = 0;
            o->a.p.whole = (short)(x - k) >> 3;
            o->y.p.whole = (short)(o->d34 - j) >> 1;
            t = DAT_800a4570[0];
            *(unsigned char *)&o->visible = 1;
            o->y.p.whole -= (t >> 8) * 4;
            FUN_80018da4(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#else
/* size 340: splat fragment 80122410 is the tail of case 0. */
#include "TOBJ.H"
extern short SCREEN_ORIGIN_X;
extern unsigned short SCREEN_ORIGIN_Y[];
extern int DAT_800a4570[];
extern int DAT_1f8002d4[];
extern void *A00_FAR_BACKDROP_WEST_ANIMS[];
extern void FUN_80018da4(TObj *);
extern void freeObjectLayer7(TObj *);

void updateA00FarBackdropWest(TObj *o)
{
    unsigned short k; int t;
    unsigned short j;
    short r;
    int x;
    switch (o->b04) {
    case 0:
        o->w1e = 11;
        o->b0d = 0;
        o->anim = A00_FAR_BACKDROP_WEST_ANIMS[o->subtype];
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        r = SCREEN_ORIGIN_X;
        k = r;
        if (r < 0x71a) {
            x = o->d30;
            j = SCREEN_ORIGIN_Y[0];
            o->b.p.whole = 0;
            o->a.p.whole = (short)(x - k) >> 3;
            o->y.p.whole = (short)(o->d34 - j) >> 1;
            t = DAT_800a4570[0];
            *(unsigned char *)&o->visible = 1;
            o->y.p.whole -= (t >> 8) * 4;
            FUN_80018da4(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#endif
