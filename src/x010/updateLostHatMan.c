// FUNC 80125c50 348 X010
// MATCHING 80125c50 348
#include "gvars.h"
#include "TOBJ.H"
extern unsigned char D_8009C941;
extern void initLostHatMan(TObj *);
extern void lostHatManTalk(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void freeObjectLayer2(TObj *);

static __inline__ int chk(void)
{
    return GVARS.b40 == 1 && D_8009C941 == 0x9f;
}

void updateLostHatMan(TObj *o)
{
    switch (o->b04) {
    case 0:
        initLostHatMan(o);
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b0c == 0x28) {
            switch (o->step) {
            case 0:
                if (o->b68 || chk()) o->step++;
                break;
            case 1:
                lostHatManTalk(o);
                break;
            }
        }
        if (o->anim && o->active) FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
