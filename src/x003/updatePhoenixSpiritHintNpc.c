// FUNC 80129be4 220 X003
// MATCHING 80129be4 220
#include "TOBJ.H"
extern void initPhoenixSpiritHintNpc(TObj *);
extern void stepPhoenixSpiritHintTalk(TObj *);
extern int FUN_800202b4(TObj *);
extern int AnimAdvance(TObj *);
extern void freeObjectLayer2(TObj *);

void updatePhoenixSpiritHintNpc(TObj *o)
{
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        initPhoenixSpiritHintNpc(o);
        break;
    case 1:
        FUN_800202b4(o);
        stepPhoenixSpiritHintTalk(o);
        break;
    case 2:
        if ((unsigned short)o->wba == 3) {
            FUN_800202b4(o);
            AnimAdvance(o);
            if (o->visible == 0)
                o->b04 = 3;
        } else {
            o->b04 = b + 1;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
