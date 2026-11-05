// FUNC 8011f634 440 X003
// MATCHING 8011f634 440
// VERSION SLPS_01144 x10 8011ED78 484
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#define PTR(p, o) (*(TObj **)((char *)(p) + (o)))
extern short objectBoxOverlapSideFacing(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
extern void spawnImpactEffect(int, short, short, short);
void func_8011ED78(TObj *o, TObj *e)
{
    short r;
    r = objectBoxOverlapSideFacing(o, e);
    if (r == -1) return;
    if (*(unsigned short *)&e->wb4 == 0 && e->subtype < 2) goto hit;
    if (e->b9c == 0 && e->subtype < 2 && e->b04 == 1) goto hit;
    switch (*(unsigned char *)&o->wac) {
    case 1:
        if (r < 3) {
            e->active = 2;
            e->b04 = 2;
            e->step = 1;
            e->state = 0;
            e->animFrame = o->animFrame & 1;
            PTR(o, 0xd4) = e;
            *(unsigned char *)&o->wac = 2;
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
    case 0:
    case 3:
    hit:
        if (e->active & 2) break;
        if (o->active & 2) break;
        o->active = 2;
        r = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = r;
        decreaseObjectTimer(o, 1);
        break;
    case 2:
        if (e->active == 3) break;
        e->active = 3;
        r = o->h->p.whole > e->h->p.whole;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->w7a = r;
        break;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"

typedef struct { TObj o; char pad[0x24]; TObj *e4; } TObjX;
extern short objectBoxOverlapSideFacing(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
extern void spawnImpactEffect(int, short, short, short);

void wingedForkPigContact(TObj *o, TObj *e)
{
    short r;

    r = objectBoxOverlapSideFacing(o, e);
    if (r == -1) return;
    switch (*(unsigned char *)&o->wac) {
    case 1:
        if (r < 3) {
            e->active = 4;
            e->b04 = 2;
            e->step = 1;
            e->state = 0;
            e->b69 = 0;
            if (e->subtype) e->w7a = e->animFrame;
            e->animFrame = o->animFrame & 1;
            ((TObjX *)o)->e4 = e;
            *(unsigned char *)&o->wac = 2;
            spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            break;
        }
    case 0:
    case 3:
        if (e->active & 2) break;
        if (SPAD.b1a4) break;
        if (o->active & 2) break;
        o->active = 2;
        r = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = r;
        decreaseObjectTimer(o, 1);
        break;
    case 2:
        if (e->active == 3) break;
        r = o->h->p.whole > e->h->p.whole;
        e->active = 3;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        e->w7a = r;
        break;
    }
}
#endif
