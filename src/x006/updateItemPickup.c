// FUNC 8011ba3c 232 X006
// MATCHING 8011ba3c 232
#include "TOBJ.H"

extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);
extern void initItemObject(TObj *o);
extern int cullObjectWideWindow(TObj *o);
extern int AnimAdvance(TObj *o);
extern void freeObjectLayer5(TObj *o);

void updateItemPickup(TObj *o)
{
    switch (o->b04) {
    case 0:
        initItemObject(o);
        o->active = 2;
        o->b04++;
        break;
    case 1:
        if (cullObjectWideWindow(o) != 0) {
            AnimAdvance(o);
        }
        break;
    case 2:
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
