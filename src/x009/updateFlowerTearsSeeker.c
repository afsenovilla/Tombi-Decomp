// FUNC 801294b4 168 X009
// MATCHING 801294b4 168
#include "TOBJ.H"
extern void initFlowerTearsSeeker(TObj *);
extern int FUN_800202b4(TObj *);
extern void stepFlowerTearsSeekerTalk(TObj *);
extern void freeObjectLayer2(TObj *);

void updateFlowerTearsSeeker(TObj *o)
{
    switch (o->b04) {
    case 0:
        initFlowerTearsSeeker(o);
        break;
    case 1:
        FUN_800202b4(o);
        stepFlowerTearsSeekerTalk(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
