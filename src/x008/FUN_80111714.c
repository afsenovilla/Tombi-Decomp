// FUNC 80111714 364 X008
// MATCHING 80111714 364
#include "TOBJ.H"
typedef struct { unsigned short a; short p0; short b; short p1; int *c; } E8;
extern E8 TBL8011[];
extern void FUN_8001fe6c(TObj *o);
extern void FUN_8001fec0(TObj *o);
typedef struct G { char p0[0xf]; unsigned char b0f; char p1[0x16 - 0x10]; unsigned short y; char p2[0x40 - 0x18]; Fix16 *h; Fix16 *d; } G;
extern G DAT_800a6038;

void FUN_80111714(TObj *o)
{
    signed char t;
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
            t = o->b6b < 0x7f ? o->b6b + 8 : o->b6b;
        } else {
            if (o->timer < 0) {
                o->b6b = 0;
                o->b04 = 3;
                goto L;
            }
            t = o->b6b != 0 ? o->b6b - 2 : 0;
        }
        o->b6b = t;
    L:
        o->h->p.whole = DAT_800a6038.h->p.whole;
        o->y.p.whole = DAT_800a6038.y - 8;
        o->d->p.whole = DAT_800a6038.d->p.whole;
        o->b0f = DAT_800a6038.b0f - 1;
    }
}
