// FUNC 801162d0 572 X016
// MATCHING 801162d0 572
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

extern void *D_8011B404[], *D_8011B410[];
extern char D_8011B468[];
extern char D_8011B470[];
extern char D_8011B478[];
extern char D_8011B480[];
static void *D_8011B3F4[4] = {
    D_8011B468, D_8011B470, D_8011B478, D_8011B480,
};
extern int DAT_1f8002d4;
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

void updateBackdropSpriteSet(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
        case 1:
            o->b0d = 1;
            o->w1e = 4;
            o->b0f = 2;
            o->anim = D_8011B3F4[o->b0c];
            break;
        case 2:
            o->b0d = 1;
            o->w1e = 4;
            o->b0f = 2;
            o->b.p.whole = 200;
            o->anim = D_8011B3F4[o->b0c];
            break;
        case 3:
            o->b0d = 1;
            o->w1e = 4;
            o->b0f = 2;
            o->anim = D_8011B404[o->b0c];
            break;
        case 4:
            o->b0d = 1;
            o->w1e = 4;
            o->b0f = 2;
            o->anim = D_8011B410[o->b0c];
            break;
        }
        o->d3c = DAT_1f8002d4;
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype != 2 || GCD.bce05 == 0xff) {
            o->visible = 1;
            o->a.p.whole = o->d30 - ((unsigned short*)((char*)&SPAD.i174 + 2))[0];
            o->y.p.whole = o->d34 - ((unsigned short*)&SPAD.s186)[0];
            FUN_80018ca4(o);
            FUN_8001fec0(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
