// FUNC 80111714 364 X000
#include "TOBJ.H"
typedef struct { unsigned short a; short p0; short b; short p1; int *c; } E8;
extern E8 TBL8011[];
extern void FUN_8001fe6c(TObj *o);
extern void FUN_8001fec0(TObj *o);
extern Fix16 *DAT_800a607c;
extern Fix16 *DAT_800a6078;
extern unsigned short DAT_800a604e;
extern unsigned char DAT_800a6047;

void FUN_80111714(TObj *o)
{
    unsigned char t;
    switch (o->state) {
    case 0:
        o->anim = (void *)TBL8011[o->subtype].c[o->b0c];
        FUN_8001fe6c(o);
        o->ba5 = 1;
        o->d88 = 0;
        o->timer = 0xb4;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->timer--;
        o->visible = 0;
        if (o->timer >= 0x3d) {
            unsigned char u = o->b6b;
            t = u + 8;
            if (u > 0x7e) t = u;
        } else {
            if (o->timer < 0) {
                o->b6b = 0;
                o->b04 = 3;
                goto L;
            }
            t = o->b6b - 2;
            if (o->b6b == 0) t = 0;
        }
        o->b6b = t;
    L:
        o->h->p.whole = DAT_800a6078->p.whole;
        o->y.p.whole = DAT_800a604e - 8;
        o->d->p.whole = DAT_800a607c->p.whole;
        o->b0f = DAT_800a6047 - 1;
    }
}
