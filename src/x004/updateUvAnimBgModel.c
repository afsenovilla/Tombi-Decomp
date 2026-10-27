// FUNC 80119114 792 X004
// MATCHING 80119114 792
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void FUN_80018d2c(TObj *);
extern void uvAnimBgModelFlip(TObj *);
extern void uvAnimBgModelStepB(TObj *);
extern void uvAnimBgModelStepC(TObj *);
extern void freeObjectLayer8(TObj *);

void updateUvAnimBgModel(TObj *o)
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
            if (GVARS.hurtState) goto set2;
            k = GAME.selectedSection;
            if (k == 0 && (unsigned short)((*(unsigned short*)((char*)&SPAD.i174 + 2)) - 0x1d5) < 0x191) break;
            if (k == 1 && (unsigned short)((*(unsigned short*)((char*)&SPAD.i174 + 2)) - 0x191) < 0x207 && (*(short*)&SPAD.s186) < -0xa0) break;
            o->visible = 1;
            FUN_80018d2c(o);
            uvAnimBgModelFlip(o);
            break;
        case 1:
            if (GVARS.hurtState) goto set2;
            k = GAME.selectedSection;
            if (k == 0 && (short)(*(unsigned short*)((char*)&SPAD.i174 + 2)) < 0x30c) break;
            if (k == 1 && (short)(*(unsigned short*)((char*)&SPAD.i174 + 2)) >= 0x201) break;
            o->visible = 1;
            FUN_80018d2c(o);
            uvAnimBgModelStepB(o);
            break;
        case 2:
            if (GVARS.hurtState) {
            set2:
                o->b04 = 2;
                o->step = 0;
                break;
            }
            o->visible = 1;
            FUN_80018d2c(o);
            uvAnimBgModelStepC(o);
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
                uvAnimBgModelFlip(o);
            }
            break;
        case 1:
            if (o->w22 & 3) {
                o->state = 2;
                uvAnimBgModelStepB(o);
            }
            break;
        case 2:
            if (o->w22 & 3) {
                o->state = 2;
                uvAnimBgModelStepC(o);
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
