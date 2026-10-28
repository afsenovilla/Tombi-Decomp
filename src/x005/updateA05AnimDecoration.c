// FUNC 801161a8 268 X005
// MATCHING 801161a8 268
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern char D_8011A470[];
extern char D_8011A478[];
extern char D_8011A480[];
extern char D_8011A488[];
extern char D_8011A490[];
extern char D_8011A498[];
extern char D_8011A4A0[];
extern char D_8011A4A8[];
extern char D_8011A4B0[];
extern char D_8011A4F0[];
extern char D_8011A508[];
extern char D_8011A52C[];
extern char D_8011A550[];
extern char D_8011A570[];
extern char D_8011A578[];
extern char D_8011A5E8[];
extern char D_8011A5F8[];
static void *D_8011A42C[17] = {
    D_8011A470, D_8011A478, D_8011A480, D_8011A488,
    D_8011A490, D_8011A498, D_8011A4A0, D_8011A4A8,
    D_8011A4B0, D_8011A4F0, D_8011A508, D_8011A52C,
    D_8011A550, D_8011A570, D_8011A578, D_8011A5E8,
    D_8011A5F8,
};
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void updateA05AnimDecoration(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GCD.bd081 < 6) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box3 = 0x10;
        o->box2 = 8;
        o->b0d = 0;
        o->w1e = 7;
        o->d3c = ((int*)&SPAD.i2f0)[0];
        o->anim = D_8011A42C[o->b0c];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
