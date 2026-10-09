// FUNC 801166f0 432 X018
// MATCHING 801166f0 432
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } P4;

extern int D_1F8002D0[];
extern void *D_8011CA48[];
extern short D_8011A4A0[];
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

#define PT(o) ((P4 *)((char *)(o) + 0xb4))

void func_801166F0(TObj *o)
{
    unsigned char t = o->b04;
    unsigned char u;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->b0f = 0x40;
        o->b0d = 0x81;
        o->w1e = 0xb;
        o->w08 = 0x7c0c;
        o->d3c = D_1F8002D0[0];
        o->anim = D_8011CA48[o->b0c];
        break;
    case 1:
        o->visible = 1;
        FUN_80018ca4(o);
        u = o->step;
        switch (u) {
        case 0:
            o->step = u + 1;
            PT(o)[0].x = D_8011A4A0[0];
            PT(o)[0].y = D_8011A4A0[1];
            PT(o)[0].z = 0;
            PT(o)[1].x = D_8011A4A0[2];
            PT(o)[1].y = D_8011A4A0[3];
            PT(o)[1].z = 0;
            PT(o)[2].x = D_8011A4A0[4];
            PT(o)[2].y = D_8011A4A0[5];
            PT(o)[2].z = 0;
            PT(o)[3].x = D_8011A4A0[6];
            PT(o)[3].y = D_8011A4A0[7];
            PT(o)[3].z = 0;
            break;
        case 1:
            o->a.raw += 0xc000;
            if (o->a.p.whole >= 0x1e1) o->a.p.whole = -0x78;
            break;
        case 2:
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
