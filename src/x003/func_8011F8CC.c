// FUNC 8011f8cc 416 X003
// MATCHING 8011f8cc 416
#include "TOBJ.H"

typedef struct { TObj o; char pad[0x24]; TObj *e4; } TObjX;
extern unsigned char D_1F8001A4;
extern short func_800435E0(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_8011F8CC(TObj *o, TObj *e)
{
    short r;

    if (e->subtype == 4) return;
    r = func_800435E0(o, e);
    if (r == -1) return;
    switch (*(unsigned char *)&o->wac) {
    case 1:
        if (r < 3) {
            e->active = 4;
            e->b04 = 2;
            e->step = 1;
            e->state = 0;
            e->b69 = 0;
            e->animFrame = o->animFrame & 1;
            ((TObjX *)o)->e4 = e;
            *(unsigned char *)&o->wac = 2;
            break;
        }
    case 0:
    case 3:
        if (e->active & 2) break;
        e->b69 = 8;
        if (D_1F8001A4) break;
        if (o->active & 2) break;
        o->active = 2;
        r = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = r;
        FUN_8004258c(o, 1);
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
