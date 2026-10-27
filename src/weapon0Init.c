// FUNC 80031664 348 MAIN0
// MATCHING 80031664 348
// VERSION SLPM_80165 slpm80165 8002F308 320
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80031664 (FUN_80031664.c):  Inline GetClut, CLUT rows 0x1e8/0x1e9 (PAL 0x1ef/0x1f0).
#include "TOBJ.H"
extern short *WEAPON_STATE;
extern short WEAPON_BEND_PARAMS[], DAT_8009d602[], DAT_8009d604[];
extern Fix16 *PLAYER_H;
extern Fix16 *PLAYER_D;
extern unsigned short PLAYER_Y[];
extern int GetGraphType(void);
extern unsigned short GetClut(int, int);

void FUN_80031664(TObj *o)
{
    short *p;
    int v;
    WEAPON_STATE = &o->wac;
    if (o->state == 0) {
        if (GetGraphType() != 1) GetGraphType();
        p = WEAPON_STATE;
        o->wb0 = 0;
        if (GetGraphType() == 1 || GetGraphType() == 2) v = 0x80; else v = 0x20;
        p[4] = v;
        WEAPON_STATE[3] = ((0x1e8 << 6) | (0x80 >> 4));
        WEAPON_STATE[5] = ((0x1e9 << 6) | (0x80 >> 4));
        WEAPON_BEND_PARAMS[0] = 0xf;
        DAT_8009d602[0] = 8;
        DAT_8009d604[0] = 0x5a;
        *(signed char *)&o->b0f = -9;
        o->b0a = 0;
        o->h->p.whole = PLAYER_H->p.whole;
        o->y.p.whole = PLAYER_Y[0] - 0x10;
        o->d->p.whole = PLAYER_D->p.whole;
        o->velH = 0;
        o->velV = 0;
        o->animFrame = 0;
        o->visible = 1;
        o->state = 0;
    }
}
#else
#include "TOBJ.H"
extern short *WEAPON_STATE;
extern short WEAPON_BEND_PARAMS[], DAT_8009d602[], DAT_8009d604[];
extern Fix16 *PLAYER_H;
extern Fix16 *PLAYER_D;
extern unsigned short PLAYER_Y[];
extern int GetGraphType(void);
extern unsigned short GetClut(int, int);

void weapon0Init(TObj *o)
{
    short *p;
    int v;
    WEAPON_STATE = &o->wac;
    if (o->state == 0) {
        if (GetGraphType() != 1) GetGraphType();
        p = WEAPON_STATE;
        o->wb0 = 0;
        if (GetGraphType() == 1 || GetGraphType() == 2) v = 0x80; else v = 0x20;
        p[4] = v;
        WEAPON_STATE[3] = GetClut(0x80, 0x1ef);
        WEAPON_STATE[5] = GetClut(0x80, 0x1f0);
        WEAPON_BEND_PARAMS[0] = 0xf;
        DAT_8009d602[0] = 8;
        DAT_8009d604[0] = 0x5a;
        *(signed char *)&o->b0f = -9;
        o->b0a = 0;
        o->h->p.whole = PLAYER_H->p.whole;
        o->y.p.whole = PLAYER_Y[0] - 0x10;
        o->d->p.whole = PLAYER_D->p.whole;
        o->velH = 0;
        o->velV = 0;
        o->animFrame = 0;
        o->visible = 1;
        o->state = 0;
    }
}
#endif
