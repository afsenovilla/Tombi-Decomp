// FUNC 801165a8 328 X018
// MATCHING 801165a8 328
#include "spad.h"
#include "TOBJ.H"

extern char D_8011CBF0[];
extern char D_8011CC10[];
extern char D_8011CC18[];
extern char D_8011CC4C[];
extern char D_8011CC54[];
extern char D_8011CC68[];
extern char D_8011CC98[];
extern char D_8011CCA0[];
extern char D_8011CCB0[];
extern char D_8011CCD8[];
extern char D_8011CCE0[];
extern char D_8011CCFC[];
extern char D_8011CD34[];
extern char D_8011CD3C[];
extern char D_8011CD44[];
extern char D_8011CD4C[];
static void *D_8011CA8C[16] = {
    D_8011CBF0, D_8011CC10, D_8011CC18, D_8011CC4C,
    D_8011CC54, D_8011CC68, D_8011CC98, D_8011CCA0,
    D_8011CCB0, D_8011CCD8, D_8011CCE0, D_8011CCFC,
    D_8011CD34, D_8011CD3C, D_8011CD44, D_8011CD4C,
};
extern unsigned short FUN_8005e420(int, int);
extern void AnimLoadDuration(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void updateAnimBackdropSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->b0a = 7;
        o->b0d = 1;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        o->w1e = 4;
        o->anim = D_8011CA8C[o->b0c];
        o->d3c = ((int*)&SPAD.i2d4)[0];
        AnimLoadDuration(o);
        break;
    case 1:
        o->a.p.whole = o->d30 - (*(unsigned short*)((char*)&SPAD.i174 + 2));
        {
            int y = o->d34;
            o->y.p.whole = y - ((unsigned short*)&SPAD.s186)[0];
        }
        o->visible = 1;
        ObjListPush_1F800220(o);
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
