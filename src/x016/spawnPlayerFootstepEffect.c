// FUNC 800eef0c 336 X016
// MATCHING 800eef0c 336
#include "TOBJ.H"
extern unsigned char DAT_8009c990;
extern unsigned short TICK_COUNT;
extern short FREE_OBJECT_COUNT;
extern TObj *FUN_80018448(void);
extern unsigned short PLAYER_X[];
extern unsigned short PLAYER_FOOT_OFFSET_BY_DIR[];

short spawnPlayerFootstepEffect(void)
{
    short s = 0;
    TObj *o;
    switch (DAT_8009c990) {
    case 1:
        s = 5;
        break;
    case 2:
        s = (TICK_COUNT & 1) + 5;
        break;
    case 3:
        s = 1;
        break;
    }
    if (s != 0) {
        if (FREE_OBJECT_COUNT < 6) return 0;
        o = FUN_80018448();
        if (o != 0) {
            unsigned short *t;
            Fix16 *h = o->h;
            o->active = 1;
            o->type = 0x4e;
            o->subtype = DAT_8009c990 - 1;
            o->b0c = s;
            o->a.p.whole = PLAYER_X[0];
            o->y.p.whole = PLAYER_X[2];
            o->b.p.whole = PLAYER_X[4];
            t = PLAYER_FOOT_OFFSET_BY_DIR + ((TICK_COUNT + 2) & 7) * 2;
            h->p.whole += t[0];
            o->y.p.whole += t[1];
            o->step = 1;
        }
    }
    return s;
}
