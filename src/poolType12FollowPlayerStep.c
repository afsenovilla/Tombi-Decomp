// FUNC 8002c238 516 MAIN0
// MATCHING 8002c238 516
// VERSION SLPS_01144 slps01144 8002CE10 516
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 8002C238 (poolType12FollowPlayerStep.c): 71 -> 85.
#include "TOBJ.H"
extern unsigned char DAT_800a6039;
extern unsigned char PLAYER_SUBSTATE;
extern short DAT_800a60b6;
extern unsigned char DAT_800a60d4;
extern void *DAT_8001473c;
extern void *DAT_80014740;
extern unsigned short *DAT_800a605c[];
extern int DAT_800a60c4;
extern unsigned short DAT_800a6066;
extern Fix16 *PLAYER_H[];
extern Fix16 *PLAYER_D[];
extern unsigned short PLAYER_Y[];
extern unsigned char DAT_800a6047[];
extern unsigned char DAT_800a6047b[];
extern signed char DAT_80011eb4[];
extern unsigned char DAT_80011dad[];
extern unsigned char DAT_800a6101[];
extern void FUN_8001fe6c(TObj *);

void poolType12FollowPlayerStep(TObj *o)
{
    int d;
    o->d8c = 0;
    o->visible = DAT_800a6039;
    if (PLAYER_SUBSTATE == 2 && DAT_800a60b6 >= -0xc7 && DAT_800a60d4 != 0) {
        o->anim = DAT_8001473c;
        FUN_8001fe6c(o);
        if (DAT_800a60b6 > 0 && *DAT_800a605c[0] == 0x55)
            o->d8c = DAT_800a60c4;
    } else if (PLAYER_SUBSTATE == 4 && DAT_800a60b6 >= -0xc7) {
        o->anim = DAT_8001473c;
        FUN_8001fe6c(o);
        if (DAT_800a60b6 > 0)
            o->d8c = DAT_800a60c4;
    } else {
        o->anim = DAT_80014740;
        FUN_8001fe6c(o);
        d = DAT_800a60c4;
        o->d8c = (unsigned char)((DAT_800a6066 & 1) ? d - 0x20 : d + 0x20);
    }
    o->h->p.whole = PLAYER_H[0]->p.whole;
    o->y.p.whole = PLAYER_Y[0] + 4;
    o->d->p.whole = PLAYER_D[0]->p.whole;
    o->b0f = DAT_800a6047[0] + 1;
    o->b0f = DAT_800a6047b[0] + DAT_80011dad[DAT_80011eb4[*DAT_800a605c[0]] * 4];
    if (DAT_800a6101[0] == 0)
        o->b04 = 3;
}
#else
#include "TOBJ.H"
extern unsigned char DAT_800a6039;
extern unsigned char PLAYER_SUBSTATE;
extern short DAT_800a60b6;
extern unsigned char DAT_800a60d4;
extern void *DAT_8001473c;
extern void *DAT_80014740;
extern unsigned short *DAT_800a605c[];
extern int DAT_800a60c4;
extern unsigned short DAT_800a6066;
extern Fix16 *PLAYER_H[];
extern Fix16 *PLAYER_D[];
extern unsigned short PLAYER_Y[];
extern unsigned char DAT_800a6047[];
extern unsigned char DAT_800a6047b[];
extern signed char DAT_80011eb4[];
extern unsigned char DAT_80011dad[];
extern unsigned char DAT_800a6101[];
extern void FUN_8001fe6c(TObj *);

void poolType12FollowPlayerStep(TObj *o)
{
    int d;
    o->d8c = 0;
    o->visible = DAT_800a6039;
    if (PLAYER_SUBSTATE == 2 && DAT_800a60b6 >= -0xc7 && DAT_800a60d4 != 0) {
        o->anim = DAT_8001473c;
        FUN_8001fe6c(o);
        if (DAT_800a60b6 > 0 && *DAT_800a605c[0] == 0x47)
            o->d8c = DAT_800a60c4;
    } else if (PLAYER_SUBSTATE == 4 && DAT_800a60b6 >= -0xc7) {
        o->anim = DAT_8001473c;
        FUN_8001fe6c(o);
        if (DAT_800a60b6 > 0)
            o->d8c = DAT_800a60c4;
    } else {
        o->anim = DAT_80014740;
        FUN_8001fe6c(o);
        d = DAT_800a60c4;
        o->d8c = (unsigned char)((DAT_800a6066 & 1) ? d - 0x20 : d + 0x20);
    }
    o->h->p.whole = PLAYER_H[0]->p.whole;
    o->y.p.whole = PLAYER_Y[0] + 4;
    o->d->p.whole = PLAYER_D[0]->p.whole;
    o->b0f = DAT_800a6047[0] + 1;
    o->b0f = DAT_800a6047b[0] + DAT_80011dad[DAT_80011eb4[*DAT_800a605c[0]] * 4];
    if (DAT_800a6101[0] == 0)
        o->b04 = 3;
}
#endif
