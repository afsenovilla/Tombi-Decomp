// FUNC 8011b8a4 408 X006
// MATCHING 8011b8a4 408
#include "camera.h"
#include "spad.h"
#include "TOBJ.H"

extern char D_80122F68[];
extern char D_80122F70[];
extern char D_80122F78[];
extern char D_80122F80[];
extern char D_80122F88[];
extern char D_80122F90[];
extern char D_80122F98[];
extern char D_80122FA0[];
extern char D_80122FA8[];
static void *D_80122E2C[9] = {
    D_80122F68, D_80122F70, D_80122F78, D_80122F80,
    D_80122F88, D_80122F90, D_80122F98, D_80122FA0,
    D_80122FA8,
};
void ObjListPush_1F80022C(TObj *o);
void freeObjectLayer7(TObj *o);

void updateParallaxBackdropSprite(TObj *o)
{
    short x;
    int x0, t4, t0;
    unsigned short k, j;
    switch (o->b04) {
    case 0:
        o->w1e = 14;
        o->y.p.whole += 0x60;
        o->b0d = 0;
        o->anim = D_80122E2C[o->subtype];
        o->b0f = 3;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        x0 = o->d30;
        k = (*(unsigned short*)((char*)&SPAD.i174 + 2));
        t4 = CAMERA_SCROLL.rotY;
        j = ((unsigned short*)&SPAD.s186)[0];
        t0 = CAMERA_SCROLL.rotX;
        o->b.p.whole = 0;
        o->a.p.whole = (short)(x0 - k + ((t4 >> 8) << 2)) >> 3;
        o->y.p.whole = (short)(o->d34 - j - ((t0 >> 8) << 3)) >> 2;
        x = o->a.p.whole;
        if (x - 0x40 < 0x141 && x + 0x40 >= 0) {
            if (o->subtype != 5 || (unsigned short)((*(unsigned short*)((char*)&SPAD.iec + 2)) - 0x7d0) < 0x385) {
                o->visible = 1;
                ObjListPush_1F80022C(o);
            }
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
