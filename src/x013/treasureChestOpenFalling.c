// FUNC 801125e8 636 X013
// MATCHING 801125e8 636
#include "TOBJ.H"
static char DAT_80115d28[20] = {
    3, 7, -0x80, 4, 0, 0, 3, 7, -0x80, 5, 0, 0, -1,
};
extern void **TREASURE_CHEST_A3S9_ANIM_SETS[];
extern void **TREASURE_CHEST_ANIM_SETS[];
extern int GAME[];
extern unsigned short DAT_8009c960h[];
extern int FUN_800202b4(TObj *o);
extern int FUN_80040278(TObj *o, int x, int y);
extern void spawnItemFromEntry(char *d, Fix16 *pos, int a, int b, TObj *o);
extern void FUN_8001fe6c(TObj *o);
extern void despawnObjectOffscreen(TObj *o);
extern void spawnBreakPieces(TObj *o, int x, int y, int z);

void treasureChestOpenFalling(TObj *o)
{
    void **p;
    FUN_800202b4(o);
    switch (o->step) {
    case 0:
        o->velV += 0x20;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        break;
    case 1:
        spawnItemFromEntry(DAT_80115d28, &o->a, -0x40, -0x400, o);
        spawnItemFromEntry(DAT_80115d28 + 6, &o->a, 0x40, -0x400, o);
        o->step = 3;
        o->timer = 10;
        o->wac = 1;
        if (GAME[0] == 0x30009)
            o->anim = TREASURE_CHEST_A3S9_ANIM_SETS[o->b0c & 0x7f][1];
        else
            o->anim = TREASURE_CHEST_ANIM_SETS[DAT_8009c960h[0] * 4 + (o->b0c & 0x7f)][1];
        FUN_8001fe6c(o);
        break;
    case 2:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        break;
    case 3:
        if (--o->timer == -1) {
            o->active = 3;
            o->velV = 0;
            o->step++;
        }
        break;
    case 4:
        o->velV += 0x20;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        despawnObjectOffscreen(o);
        break;
    case 7:
        spawnBreakPieces(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->b04++;
        break;
    }
}
