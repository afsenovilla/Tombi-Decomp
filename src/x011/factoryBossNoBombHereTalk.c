// FUNC 801187d4 548 X011
// MATCHING 801187d4 548
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
extern TObj PLAYER;
extern unsigned char GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID;
extern unsigned char DAT_8009c942[];
extern unsigned char GAME_PLAYER_MODE;
extern short D_1F8001C6;
extern AT3 D_80119C48[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void AnimLoadDuration(TObj *);

void factoryBossNoBombHereTalk(TObj *o)
{
    B12 v;
    unsigned char *c;

    switch (o->state) {
    case 0:
        c = &GVARS_USE_ITEM_PENDING;
        if (*c != 0) {
            if (GVARS_USE_ITEM_ID != 0x3c)
                break;
            o->step = 5;
            o->state = 0;
            *c = 0;
            break;
        }
        if (o->b68 == 0)
            break;
        DAT_8009c942[0] = 1;
        GAME_PLAYER_MODE = 2;
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[2];
        AnimLoadDuration(o);
        o->d90 = (int)showMessageBoxDirect(2, 0x10, &v);
        o->state++;
        break;
    case 1:
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        AnimLoadDuration(o);
        o->state++;
        break;
    case 2:
        DAT_8009c942[0] = 0;
        PLAYER.wb2 = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
