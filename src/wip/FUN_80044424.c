// FUNC 80044424 300 MAIN0
#include "TOBJ.H"
extern short DAT_1f80019e;
unsigned int FUN_80044424(TObj *param_1, TObj *param_2)
{
    int iVar1, iVar2, iVar3, iVar4;
    if ((unsigned short)((unsigned short)(param_1->d->p).whole - (unsigned short)(param_2->d->p).whole + 0x2d) < 0x5b) {
        iVar1 = (unsigned short)param_2->box0 + (unsigned short)param_1->box0;
        iVar3 = (unsigned short)(param_1->h->p).whole - (unsigned short)(param_2->h->p).whole;
        if ((int)(unsigned short)(iVar3 + iVar1) <= (int)param_2->box1 + (int)param_1->box1) {
            iVar2 = (unsigned short)(param_1->y).p.whole - (unsigned short)(param_2->y).p.whole;
            if ((int)(unsigned short)(iVar2 + (unsigned short)param_2->box2 + (unsigned short)param_1->box2) <= (int)param_1->box3 + (int)param_2->box3) {
                DAT_1f80019e = 0;
                if (iVar3 * 0x10000 < 0) {
                    iVar4 = -iVar3;
                } else {
                    iVar1 = ((unsigned short)param_2->box1 - (unsigned short)param_2->box0) + ((unsigned short)param_1->box1 - (unsigned short)param_1->box0);
                    iVar4 = iVar3;
                }
                if (3 < (unsigned short)(iVar1 - iVar4)) {
                    if (0 < iVar2 * 0x10000) return 2;
                    return 3;
                }
                return (unsigned int)~(int)(short)iVar3 >> 31;
            }
        }
    }
    return -1;
}
