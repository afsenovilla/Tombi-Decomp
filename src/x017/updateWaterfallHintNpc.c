// FUNC 80118014 168 X017
// MATCHING 80118014 168
#include "TOBJ.H"
extern void initWaterfallHintNpc(TObj *);
extern int FUN_800202b4(TObj *);
extern void stepWaterfallHintNpcTalk(TObj *);
extern void freeObjectLayer2(TObj *);

void updateWaterfallHintNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        initWaterfallHintNpc(o);
        break;
    case 1:
        FUN_800202b4(o);
        stepWaterfallHintNpcTalk(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
