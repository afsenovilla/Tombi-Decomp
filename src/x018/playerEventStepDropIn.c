// FUNC 8010a9f4 668 X018
// MATCHING 8010a9f4 668
#include "TOBJ.H"
extern void playerReleaseBittenAndClearFlags(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void spawnPlayerPoolType13Object(TObj *, int, int);
extern TObj *allocObjectLayer2(void);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);
extern TObj *PLAYER_EXT;
extern char DAT_80010ae8[];
extern char DAT_80010748[];
extern int GAME[];
extern unsigned char DAT_8009c942;
extern unsigned char DAT_8009c93f;
extern unsigned char GVARS_PLAYER_LANDED[];
extern unsigned char GAME_PLAYER_MODE[];
extern unsigned char DAT_8009cff9;
extern unsigned char DAT_8009c942b[];
extern unsigned char DAT_8009c93fb[];
extern unsigned char SLOPE_TILT_TABLE[];

void playerEventStepDropIn(TObj *o)
{
    TObj *p;
    TObj *q;

    switch (o->state) {
    case 0:
        o->active = 5;
        o->d8c = 0;
        PLAYER_EXT->timer = 0;
        *(unsigned char *)&PLAYER_EXT->w08 = 0;
        PLAYER_EXT->animFrame = 0xffff;
        *(unsigned short *)&PLAYER_EXT->movetab = 0xffff;
        *((unsigned short *)&PLAYER_EXT->movetab + 1) = 0xffff;
        o->ba4 = 0;
        o->ba5 = 0;
        o->b9c = 2;
        *(unsigned char *)&o->wac = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0x1000;
        PLAYER_EXT->timer = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->d8c = 0;
        o->visible = 1;
        o->w22 = 0;
        playerReleaseBittenAndClearFlags(o);
        o->anim = DAT_80010ae8;
        FUN_8001fe94(o, 3);
        DAT_8009c942 = 1;
        DAT_8009c93f = 1;
        if (GAME[0] == 0x30000) spawnPlayerPoolType13Object(o, 0, 0);
        o->state++;
        if (DAT_8009cff9 != 0) {
            p = allocObjectLayer2();
            if (p != 0) {
                p->active = 2;
                p->type = 0x54;
                p->subtype = 1;
            }
            o->state = 3;
        }
        break;
    case 1:
        FUN_8001fec0(o);
        *(unsigned char *)&PLAYER_EXT->w08 = 1;
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->velY -= 0x3000;
        if (o->velY < 0x200) o->velY = 0x200;
        FUN_8001fd94(o);
        if (o->y.p.whole >= *(short *)((char *)o + 0xf2)) {
            o->y.p.whole = *(short *)((char *)o + 0xf2);
            o->velX = 0;
            o->velY = 0;
            o->active = 4;
            FUN_8003fd78(o, 4, 0);
            o->anim = DAT_80010748;
            FUN_8001fe94(o, 0);
            FUN_8001e5f4(0x1c, 0x7f);
            o->state++;
        }
        break;
    case 2:
        o->d8c = 0;
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->d8c = SLOPE_TILT_TABLE[o->wb0];
        GVARS_PLAYER_LANDED[0] = 1;
        GAME_PLAYER_MODE[0] = 0;
        DAT_8009c942b[0] = 0;
        DAT_8009c93fb[0] = 0;
        break;
    }
}
