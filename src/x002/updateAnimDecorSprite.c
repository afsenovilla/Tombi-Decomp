// FUNC 801165dc 480 X002
// MATCHING 801165dc 480
#include "spad.h"
#include "TOBJ.H"

extern char D_8011F28C[];
extern char D_8011F294[];
extern char D_8011F29C[];
extern char D_8011F2A4[];
extern char D_8011F2DC[];
extern char D_8011F2E4[];
extern char D_8011F2EC[];
extern char D_8011F2F4[];
extern char D_8011F2FC[];
extern char D_8011F304[];
extern char D_8011F30C[];
static void *D_8011F260[11] = {
    D_8011F28C, D_8011F294, D_8011F29C, D_8011F2A4,
    D_8011F2DC, D_8011F2E4, D_8011F2EC, D_8011F2F4,
    D_8011F2FC, D_8011F304, D_8011F30C,
};
extern void *D_8011F184;
extern unsigned short GetClut(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void updateAnimDecorSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 1;
        o->animFrame = 1;
        o->b0a = 0;
        switch (o->subtype) {
        case 0:
            o->w1e = 4;
            o->w08 = GetClut(0x80, 0x1ff);
            o->anim = D_8011F260[o->subtype];
            *(signed char *)&o->b0f = -10;
            break;
        case 1:
            o->w1e = 4;
            o->w08 = GetClut(0x80, 0x1ff);
            o->anim = D_8011F260[o->subtype];
            o->b0f = 0;
            o->b0d = 0x81;
            break;
        case 2:
        case 3:
            o->w1e = 4;
            o->w08 = GetClut(0x80, 0x1ff);
            o->anim = D_8011F260[o->subtype];
            o->b0f = 0;
            break;
        case 4:
            o->w1e = 10;
            o->w08 = GetClut(0x80, 0x1fe);
            o->b0f = 0;
            o->anim = D_8011F184;
            break;
        }
        o->d3c = SPAD.i2d4;
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype != 4 && ObjCullRegister(o))
            AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
