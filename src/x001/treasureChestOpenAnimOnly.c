// FUNC 801133b8 204 X001
// MATCHING 801133b8 204
#include "TOBJ.H"
extern int GAME[];
extern unsigned short DAT_8009c960_u[];
extern char D_8012E084[];
extern char D_8012E0BC[];
extern char D_8012E0A0[];
extern char D_8012E0D8[];
static int *DAT_80115a08[4] = {
    (int *)D_8012E084, (int *)D_8012E0BC, (int *)D_8012E0A0, (int *)D_8012E0D8,
};
extern int *DAT_80115948[];
extern void FUN_800202b4();
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e560(int, int);

void treasureChestOpenAnimOnly(TObj *o)
{
    FUN_800202b4();
    if (o->step == 0) {
        o->step++;
        o->wac = 1;
        if (GAME[0] == 0x30009)
            o->anim = (void *)DAT_80115a08[o->b0c & 0x7f][1];
        else
            o->anim = (void *)DAT_80115948[DAT_8009c960_u[0] * 4 + (o->b0c & 0x7f)][1];
        FUN_8001fe6c(o);
        FUN_8001e560(0x18, 8);
    }
}
