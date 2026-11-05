// FUNC 8012d280 664 X003
// MATCHING 8012d280 664
// VERSION SLPS_01144 x03 8012C438 696
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009CF03;
extern unsigned char D_8009E4E7_[], D_8009E4EA_[], D_800A7B7C_[], D_800A7B7D_[], D_800A7B7E_[];
#define D_8009E4E7 D_8009E4E7_[0]
#define D_8009E4EA D_8009E4EA_[0]
#define D_800A7B7C D_800A7B7C_[0]
#define D_800A7B7D D_800A7B7D_[0]
#define D_800A7B7E D_800A7B7E_[0]
extern void *D_801399C8;
extern void *D_801399BC;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int FUN_800202b4(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void freeObjectLayer2(TObj *);
extern void windblownMonkeyIdle(TObj *);
extern void windblownMonkeyAskTrousers(TObj *);
extern void windblownMonkeyGiveUmbrella(TObj *);
extern void windblownMonkeyRepeatTalk(TObj *);

#define SET_ANIM(o, a) do { (o)->anim = (a); AnimLoadDuration(o); } while (0)

void func_8012C438(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        if (GCD.bce51 == 0xff) {
            o->b04 = 3;
            break;
        }
        o->active = 2;
        o->box0 = 10;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->animFrame = 1;
        o->d3c = ((int*)&SPAD.i2d0)[0];
        o->w1e = 1;
        *(signed char *)&o->b0f = -9;
        o->b0d = 0;
        o->b6a = 0;
        o->b0a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->d8c = 0;
        o->wac = 8;
        SET_ANIM(o, D_801399C8);
        o->b04++;
        if ((o->step = D_8009CF03) == 2) {
            o->a.p.whole = 0x842;
            o->y.p.whole = -0x38e;
        }
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            windblownMonkeyIdle(o);
            break;
        case 1: {
            V6 v;
            v = *(V6 *)&o->a;
            switch (o->state) {
            case 0:
                ObjSetFacingToPlayer(o);
                o->wac = 5;
                o->state++;
                do {} while (0); /* empty loop: keeps sp+16 out of an s-reg; no scheduling barrier on the D_8009CF03 store */
                o->anim = D_801399BC; AnimLoadDuration(o);
                D_8009CF03 = 2;
                o->d90 = showMessageBoxDirect(6, 4, &v);
                break;
            case 1:
                AnimAdvance(o);
                p = (TObj *)o->d90;
                if (p->b04 == 2) {
                    p->b04 = 3;
                    D_8009E4E7 = 0;
                    D_8009E4EA = 0;
                    D_800A7B7C = 1;
                    D_800A7B7D = 0;
                    D_800A7B7E = 0;
                    o->state = 0;
                    o->step++;
                }
                break;
            }
            break;
        }
        case 2:
            windblownMonkeyAskTrousers(o);
            break;
        case 3:
            windblownMonkeyGiveUmbrella(o);
            break;
        case 4:
            windblownMonkeyRepeatTalk(o);
            break;
        }
        break;
    case 2:
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#else
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009CF03;
extern void *D_801399C8;
extern void *D_801399BC;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int FUN_800202b4(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void freeObjectLayer2(TObj *);
extern void windblownMonkeyIdle(TObj *);
extern void windblownMonkeyAskTrousers(TObj *);
extern void windblownMonkeyGiveUmbrella(TObj *);
extern void windblownMonkeyRepeatTalk(TObj *);

#define SET_ANIM(o, a) do { (o)->anim = (a); AnimLoadDuration(o); } while (0)

void updateWindblownMonkey(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        if (GCD.bce51 == 0xff) {
            o->b04 = 3;
            break;
        }
        o->active = 2;
        o->box0 = 10;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->animFrame = 1;
        o->d3c = ((int*)&SPAD.i2d0)[0];
        o->w1e = 1;
        *(signed char *)&o->b0f = -9;
        o->b0d = 0;
        o->w22 = 0;
        o->b6a = 0;
        o->b0a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->d8c = 0;
        o->wac = 8;
        SET_ANIM(o, D_801399C8);
        o->b04++;
        if ((o->step = D_8009CF03) == 2) {
            o->a.p.whole = 0x842;
            o->y.p.whole = -0x38e;
        }
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            windblownMonkeyIdle(o);
            break;
        case 1: {
            V6 v;
            v = *(V6 *)&o->a;
            switch (o->state) {
            case 0:
                ObjSetFacingToPlayer(o);
                o->wac = 5;
                o->state++;
                do {} while (0); /* empty loop: keeps sp+16 out of an s-reg; no scheduling barrier on the D_8009CF03 store */
                o->anim = D_801399BC; AnimLoadDuration(o);
                D_8009CF03 = 2;
                o->d90 = showMessageBoxDirect(6, 4, &v);
                break;
            case 1:
                AnimAdvance(o);
                p = (TObj *)o->d90;
                if (p->b04 == 2) {
                    p->b04 = 3;
                    o->state = 1;
                    o->b68 = 1;
                    o->step++;
                }
                break;
            }
            break;
        }
        case 2:
            windblownMonkeyAskTrousers(o);
            break;
        case 3:
            windblownMonkeyGiveUmbrella(o);
            break;
        case 4:
            windblownMonkeyRepeatTalk(o);
            break;
        }
        break;
    case 2:
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
