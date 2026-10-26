// FUNC 801133b8 204 X000
// MATCHING 801133b8 204
#include "TOBJ.H"
extern int DAT_8009c960[];
extern unsigned short GAME[];
extern int *TREASURE_CHEST_A3S9_ANIM_SETS[];
extern int *TREASURE_CHEST_ANIM_SETS[];
extern void FUN_800202b4();
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e560(int, int);

void treasureChestOpenAnimOnly(TObj *o)
{
    FUN_800202b4();
    if (o->step == 0) {
        o->step++;
        o->wac = 1;
        if (DAT_8009c960[0] == 0x30009)
            o->anim = (void *)TREASURE_CHEST_A3S9_ANIM_SETS[o->b0c & 0x7f][1];
        else
            o->anim = (void *)TREASURE_CHEST_ANIM_SETS[GAME[0] * 4 + (o->b0c & 0x7f)][1];
        FUN_8001fe6c(o);
        FUN_8001e560(0x18, 8);
    }
}
