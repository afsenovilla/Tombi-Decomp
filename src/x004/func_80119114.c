// FUNC 80119114 792 X004
// MATCHING 80119114 792
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void FUN_80018d2c(TObj *);
extern void func_8011942C(TObj *);
extern void func_80119648(TObj *);
extern void func_80119A04(TObj *);
extern void freeObjectLayer8(TObj *);

void func_80119114(TObj *o)
{
    unsigned char t = o->b04;
    unsigned short k;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        if (o->animFrame == 1) {
            o->d84 = 0;
            o->d88 = 0x800;
            o->d8c = 0;
        }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            if (GVARS.b38) goto set2;
            k = GAME.selectedSection;
            if (k == 0 && (unsigned short)((*(unsigned short*)((char*)&SPAD.i174 + 2)) - 0x1d5) < 0x191) break;
            if (k == 1 && (unsigned short)((*(unsigned short*)((char*)&SPAD.i174 + 2)) - 0x191) < 0x207 && (*(short*)&SPAD.s186) < -0xa0) break;
            o->visible = 1;
            FUN_80018d2c(o);
            func_8011942C(o);
            break;
        case 1:
            if (GVARS.b38) goto set2;
            k = GAME.selectedSection;
            if (k == 0 && (short)(*(unsigned short*)((char*)&SPAD.i174 + 2)) < 0x30c) break;
            if (k == 1 && (short)(*(unsigned short*)((char*)&SPAD.i174 + 2)) >= 0x201) break;
            o->visible = 1;
            FUN_80018d2c(o);
            func_80119648(o);
            break;
        case 2:
            if (GVARS.b38) {
            set2:
                o->b04 = 2;
                o->step = 0;
                break;
            }
            o->visible = 1;
            FUN_80018d2c(o);
            func_80119A04(o);
            break;
        }
        break;
    case 2:
        o->visible = 1;
        FUN_80018d2c(o);
        if (o->step) break;
        switch (o->subtype) {
        case 0:
            if (o->w22 & 1) {
                o->timer = 1;
                func_8011942C(o);
            }
            break;
        case 1:
            if (o->w22 & 3) {
                o->state = 2;
                func_80119648(o);
            }
            break;
        case 2:
            if (o->w22 & 3) {
                o->state = 2;
                func_80119A04(o);
            }
            break;
        default:
            return;
        }
        o->step++;
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
