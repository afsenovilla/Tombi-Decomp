// FUNC 8011a0c0 280 X010
// MATCHING 8011a0c0 280
#include "spad.h"
#include "TOBJ.H"
typedef void (*Fn)(TObj *);

extern char D_80131E2C[];
extern char D_80131E3C[];
extern char D_80131E44[];
extern char D_80131E4C[];
extern char D_80131E34[];
extern char D_80131E54[];
extern char D_80131E5C[];
extern char D_80131ED8[];
static void *D_80131C8C[8] = {
    D_80131E2C, D_80131E3C, D_80131E44, D_80131E4C,
    D_80131E34, D_80131E54, D_80131E5C, D_80131ED8,
};
extern Fn D_8012F298[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjListPush_1F800224(TObj *);
extern void ObjFreeDup(TObj *);

void updateZiplineParts(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b69 = 0;
        o->w1e = 0xc;
        o->b0d = 0;
        o->anim = D_80131C8C[o->b0c];
        o->d3c = ((int*)&SPAD.i2d4)[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype == 3) {
            o->visible = 1;
            ObjListPush_1F800224(o);
        } else {
            ObjCullRegister(o);
        }
        D_8012F298[o->subtype](o);
        o->b69 = 0;
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
