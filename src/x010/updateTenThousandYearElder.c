// FUNC 801270ac 300 X010
// MATCHING 801270ac 300
#include "TOBJ.H"
extern void initTenThousandYearElder(TObj *);
extern void func_80125E00(TObj *);
extern void tenThousandYearElderTalk(TObj *);
extern void tenThousandYearElderBeadsTalk(TObj *);
extern void powerFishCollectorTalk(TObj *);
extern int FUN_800202b4(TObj *);
extern int AnimAdvance(TObj *);
extern void freeObjectLayer2(TObj *);

void updateTenThousandYearElder(TObj *o)
{
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        initTenThousandYearElder(o);
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            func_80125E00(o);
            break;
        case 1:
            tenThousandYearElderTalk(o);
            break;
        case 2:
            tenThousandYearElderBeadsTalk(o);
            break;
        case 3:
            powerFishCollectorTalk(o);
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04 = b + 1;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
