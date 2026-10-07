// FUNC 801269d0 148 X000
// MATCHING 801269d0 148
#include "TOBJ.H"
extern short FUN_8004b6a0(void);
extern short DAT_1f8003bc;
extern short DAT_1f80019e;
extern TObj *DAT_1f8003c0;

void FUN_801269d0(TObj *o, TObj *t)
{
    if (*(int *)((char *)t + 0x94) == 0 && FUN_8004b6a0() != 0) {
        TObj *m;
        short s;
        o->b9e = 1;
        s = DAT_1f8003bc;
        o->wba = 0xc;
        o->velY = 0;
        o->wb8 = s;
        m = t->movetab;
        m->b69 = 1;
        DAT_1f80019e = 0;
        DAT_1f8003c0 = t;
        m->animFrame = o->animFrame & 1;
    }
}
