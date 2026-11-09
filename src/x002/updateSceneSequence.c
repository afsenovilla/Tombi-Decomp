// FUNC 80118684 2412 X002
// MATCHING 80118684 2412
// VERSION SLPS_01144 x02 8011794C 2304
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { short p0, x, p4, y, p8, z; } Pos;
extern unsigned char D_8009E4E7_[];
#define D_8009E4E7 D_8009E4E7_[0]
extern unsigned char D_8009E4E3_[];
#define D_8009E4E3 D_8009E4E3_[0]
extern unsigned char D_8009E4E2_[];
#define D_8009E4E2 D_8009E4E2_[0]
extern unsigned char D_8009E51D_[];
#define D_8009E51D D_8009E51D_[0]
extern void *D_8011C5CC[], *D_8011C5D0[], *D_8011C5D4[], *D_8011C5D8[];
extern void *D_1F8002D4_[];
#define D_1F8002D4 D_1F8002D4_[0]
extern char MOVE_TABLE_0[];
extern short *D_8011BAEC[];
extern Fix16 *PLAYER_H;
extern unsigned short DAT_1f8001fc, DAT_1f8003c8;
extern TObj PLAYER;
extern unsigned char D_800A7B7E_[];
#define D_800A7B7E D_800A7B7E_[0]
extern unsigned char D_800A7B7D_[];
#define D_800A7B7D D_800A7B7D_[0]
extern unsigned char D_800A7B7C_[];
#define D_800A7B7C D_800A7B7C_[0]
extern unsigned char MOVIE_ID[];
#define D_1F8001CD MOVIE_ID[0]
extern unsigned char MOVIE_PLAY_STATE[];
#define D_1F8001CC MOVIE_PLAY_STATE[0]
extern short DRAW_FROZEN[];
#define D_1F8001C6 DRAW_FROZEN[0]
extern short D_800A7C2A_[];
#define D_800A7C2A D_800A7C2A_[0]
extern short D_800A7B8A_[];
#define D_800A7B8A D_800A7B8A_[0]
extern void AnimLoadDuration(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern int func_8002E3B4(int, int, int, int);
extern void func_8001C88C(int, int, int);
extern void func_8001C988(int, int);
extern void func_8001F050(int);
extern void func_80016740(int, void *);
extern void D_8001D5E0();
extern void func_8001EAD0(void);
extern void func_8003F550(TObj *, Pos *, int);
extern void func_80040940(TObj *, int, int);
extern void func_800EEC38(TObj *, short, short);
extern void func_80027A5C(int, int, int);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void func_80017D64(TObj *);
void func_8011794C(TObj *o)
{
    short *e;
    TObj *q;
    Pos s;
    switch (o->b04) {
    case 0:
        if (GCD.bce41 != 0) {
            o->b04 = 3;
            break;
        }
        if (GCD.bce34 == 0) o->wb4 = 0;
        else o->wb4 = 1;
        o->active = 2;
        o->wb6 = 0;
        o->w1e = 7;
        o->b0d = 0;
        {
            int d = (int)D_1F8002D4;
            void *a = D_8011C5CC[0];
            o->d3c = d;
            o->anim = a;
        }
        AnimLoadDuration(o);
        o->b0a = 0;
        o->b0f = 0;
        o->animFrame = 0;
        o->movetab = MOVE_TABLE_0;
        o->timer = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        e = D_8011BAEC[(unsigned short)o->wb4];
        e += (unsigned short)o->wb6 * 2;
        q = &PLAYER;
        switch (e[0]) {
        case 0:
            break;
        case 1:
            if (o->timer == 0) {
                o->timer = e[1];
            } else if (--o->timer == 0) {
                o->wb6++;
            }
            break;
        case 2:
            switch (o->step) {
            case 0:
                o->anim = D_8011C5D0[0];
                AnimLoadDuration(o);
                o->animFrame = 1;
                D_8009E4E7 = 1;
                q->b04 = 5;
                q->step = 100;
                q->velX = 0x100;
                q->state = 0;
                q->animFrame = 0;
                q->velY = 0x400;
                func_800EEC38(q, 1, 0);
                o->step++;
            case 1:
                if (o->h->p.whole < 200) o->animFrame = 0;
                if (o->h->p.whole >= 241) o->animFrame = 1;
                applyAnimVelocityX(o, o->animFrame);
                q->a.raw += q->velX << 8;
                q->y.raw += q->velX << 8;
                func_80040940(q, 0, 0);
                if (o->h->p.whole - PLAYER_H->p.whole < 40) {
                    D_8009E4E2 = 1;
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 3:
            switch (o->step) {
            case 0:
                o->anim = D_8011C5D0[0];
                AnimLoadDuration(o);
                o->animFrame = 1;
                o->step++;
            case 1:
                if (o->h->p.whole < 200) o->animFrame = 0;
                if (o->h->p.whole >= 241) o->animFrame = 1;
                applyAnimVelocityX(o, o->animFrame);
                if (o->h->p.whole - q->h->p.whole >= 40) break;
                if (!(DAT_1f8001fc & DAT_1f8003c8)) break;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 4:
            o->anim = D_8011C5D4[0];
            if (o->a.p.whole > D_800A7B8A) o->animFrame = 1;
            else o->animFrame = 0;
            AnimLoadDuration(o);
            o->d90 = func_8002E3B4(7, e[1], 0xdc, 0x9c);
            o->wb6++;
            break;
        case 5:
            o->anim = D_8011C5CC[0];
            if (o->a.p.whole > D_800A7B8A) o->animFrame = 1;
            else o->animFrame = 0;
            AnimLoadDuration(o);
            o->d90 = func_8002E3B4(7, e[1], 0xdc, 0x9c);
            o->wb6++;
            break;
        case 6:
            if (((unsigned char *)o->d90)[4] < 2) break;
            o->anim = D_8011C5CC[0];
            AnimLoadDuration(o);
            ((unsigned char *)o->d90)[4]++;
            o->wb6++;
            break;
        case 7:
            func_8001C88C(e[1], 0, 0);
            o->wb6++;
            break;
        case 8:
            func_8001C988(e[1], 0);
            o->wb6++;
            break;
        case 9:
            D_8009E51D = 3;
            o->wb6++;
            break;
        case 10:
            if (D_8009E51D == 1) o->wb6++;
            break;
        case 11:
            D_8009E51D = 4;
            o->wb6++;
            break;
        case 12:
            if (D_8009E51D == 0) o->wb6++;
            break;
        case 13:
            D_1F8001CC = 1;
            D_1F8001CD = 3;
            D_1F8001C6 = 2;
            func_8001F050(0);
            func_80016740(1, D_8001D5E0);
            o->wb6++;
            break;
        case 14:
            func_8001EAD0();
            o->wb6++;
            break;
        case 15:
            D_800A7B7C = 5;
            D_800A7B7D = 0;
            D_800A7B7E = 0;
            D_8009E4E7 = 1;
            o->wb6++;
            break;
        case 16:
            D_800A7C2A = 0;
            D_800A7B7C = 1;
            D_800A7B7D = 0;
            D_800A7B7E = 0;
            D_8009E4E7 = 0;
            o->wb6++;
            break;
        case 17:
            if (PLAYER_H->p.whole > 104) PLAYER_H->p.whole = 104;
            break;
        case 18:
            switch (o->step) {
            case 0:
                o->animFrame = 0;
                o->anim = D_8011C5D0[0];
                AnimLoadDuration(o);
                o->movetab = MOVE_TABLE_0;
                o->step++;
            case 1:
                applyAnimVelocityX(o, 0);
                if (o->a.p.whole < 261) break;
                o->anim = D_8011C5CC[0];
                D_8009E4E3 = 1;
                AnimLoadDuration(o);
                o->w22 = 60;
                o->wb8 = 0;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) {
                    s.x = o->h->p.whole;
                    s.y = o->y.p.whole;
                    s.z = o->d->p.whole;
                    func_8003F550(o, &s, 1);
                    o->step = 0;
                    o->animFrame = 1;
                    o->wb6++;
                }
                break;
            }
            break;
        case 19:
            switch (o->step) {
            case 0:
                o->animFrame = 1;
                o->anim = D_8011C5D0[0];
                AnimLoadDuration(o);
                o->movetab = MOVE_TABLE_0;
                o->wb8 = 1;
                o->step++;
            case 1:
                applyAnimVelocityX(o, 1);
                if (o->a.p.whole >= 176) break;
                o->anim = D_8011C5D8[0];
                AnimLoadDuration(o);
                o->w22 = 60;
                o->wb8 = 2;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) {
                    o->step = 0;
                    o->animFrame = 1;
                    o->wb6++;
                }
                break;
            }
            break;
        case 20:
            func_80027A5C(0x2d, 1, 1);
            o->wb6++;
            break;
        case 21:
            o->wb6++;
            o->step = 0;
            o->wba = o->wb6;
            break;
        case 22:
            o->wb6 = o->wba;
            break;
        }
        if (ObjCullRegister(o)) AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80017D64(o);
        break;
    }
}
#else
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { short p0, x, p4, y, p8, z; } Pos;
extern unsigned char GAME_PLAYER_HEALTH[];
#define D_8009C970 GAME_PLAYER_HEALTH[0]
extern unsigned char D_8009C93F_[];
#define D_8009C93F D_8009C93F_[0]
extern unsigned char D_8009C93B_[];
#define D_8009C93B D_8009C93B_[0]
extern unsigned char GVARS_PLAYER_LANDED[];
#define D_8009C93A GVARS_PLAYER_LANDED[0]
extern unsigned char GAME_FADE_SCREEN_CONTROL[];
#define D_8009C975 GAME_FADE_SCREEN_CONTROL[0]
extern signed char GAME_PLAYER_MODE[];
#define D_8009D2B0 GAME_PLAYER_MODE[0]
extern void *D_8011F868[], *D_8011F86C[], *D_8011F870[], *D_8011F874[];
extern void *D_1F8002D4_[];
#define D_1F8002D4 D_1F8002D4_[0]
extern char MOVE_TABLE_0[];
extern short *D_8011C8E8[];
extern Fix16 *PLAYER_H;
extern unsigned short BUTTON_MASK_A, JOYPAD_STATE;
extern TObj PLAYER;
extern unsigned char PLAYER_STEP[];
#define D_800A603E PLAYER_STEP[0]
extern unsigned char PLAYER_SUBSTATE[];
#define D_800A603D PLAYER_SUBSTATE[0]
extern unsigned char PLAYER_STATE[];
#define D_800A603C PLAYER_STATE[0]
extern unsigned char MOVIE_ID[];
#define D_1F8001CD MOVIE_ID[0]
extern unsigned char MOVIE_PLAY_STATE[];
#define D_1F8001CC MOVIE_PLAY_STATE[0]
extern short DRAW_FROZEN[];
#define D_1F8001C6 DRAW_FROZEN[0]
extern short D_800A60EA_[];
#define D_800A60EA D_800A60EA_[0]
extern short PLAYER_X[];
#define D_800A604A PLAYER_X[0]
extern short D_800A60D2_[];
#define D_800A60D2 D_800A60D2_[0]
extern short PLAYER_HEALTH[];
#define D_800A60D0 PLAYER_HEALTH[0]
extern void FUN_8001fe6c(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern int showMessageBox(int, int, int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void FUN_8001f110(int);
extern void FUN_800171b8(int, void *);
extern void FUN_8001d6a4();
extern void startAreaBgm(void);
extern void dispatchAreaItemUpdate(TObj *, Pos *, int);
extern void FUN_8003fd78(TObj *, int, int);
extern void playerSetAnim(TObj *, short, short);
extern void FUN_80026c50(int, int, int);
extern void FUN_8001e4f0(int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void freeObjectLayer2(TObj *);

void updateSceneSequence(TObj *o)
{
    short *e;
    TObj *q;
    Pos s;

    switch (o->b04) {
    case 0:
        if (GCD.bce41 != 0) {
            o->b04 = 3;
            break;
        }
        if (GCD.bce34 == 0) o->wb4 = 0;
        else if (D_8009C970 < GAME.playerMaxHealth) o->wb4 = 2;
        else o->wb4 = 1;
        o->active = 2;
        o->wb6 = 0;
        o->w1e = 7;
        o->b0d = 0;
        {
            int d = (int)D_1F8002D4;
            void *a = D_8011F868[0];
            o->d3c = d;
            o->anim = a;
        }
        FUN_8001fe6c(o);
        o->b0a = 0;
        o->b0f = 0;
        o->animFrame = 0;
        o->movetab = MOVE_TABLE_0;
        o->timer = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        e = D_8011C8E8[(unsigned short)o->wb4];
        e += (unsigned short)o->wb6 * 2;
        q = &PLAYER;
        switch (e[0]) {
        case 0:
            break;
        case 1:
            if (o->timer == 0) {
                o->timer = e[1];
            } else if (--o->timer == 0) {
                o->wb6++;
            }
            break;
        case 2:
            switch (o->step) {
            case 0:
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->animFrame = 1;
                D_8009C93F = 1;
                q->b04 = 5;
                q->step = 100;
                q->velX = 0x100;
                q->state = 0;
                q->animFrame = 0;
                q->velY = 0x400;
                playerSetAnim(q, 1, 0);
                o->step++;
            case 1:
                if (o->h->p.whole < 200) o->animFrame = 0;
                if (o->h->p.whole >= 241) o->animFrame = 1;
                applyAnimVelocityX(o, o->animFrame);
                q->a.raw += q->velX << 8;
                q->y.raw += q->velX << 8;
                FUN_8003fd78(q, 0, 0);
                if (o->h->p.whole - PLAYER_H->p.whole < 40) {
                    D_8009C93A = 1;
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 3:
            switch (o->step) {
            case 0:
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->animFrame = 1;
                o->step++;
            case 1:
                if (o->h->p.whole < 200) o->animFrame = 0;
                if (o->h->p.whole >= 241) o->animFrame = 1;
                applyAnimVelocityX(o, o->animFrame);
                if (D_8009D2B0 == 3) break;
                if (o->h->p.whole - q->h->p.whole >= 40) break;
                if (!(JOYPAD_STATE & BUTTON_MASK_A)) break;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 4:
            o->anim = D_8011F870[0];
            if (o->a.p.whole > D_800A604A) o->animFrame = 1;
            else o->animFrame = 0;
            FUN_8001fe6c(o);
            o->d90 = showMessageBox(7, e[1], 0xdc, 0x9c);
            o->wb6++;
            break;
        case 5:
            o->anim = D_8011F868[0];
            if (o->a.p.whole > D_800A604A) o->animFrame = 1;
            else o->animFrame = 0;
            FUN_8001fe6c(o);
            o->d90 = showMessageBox(7, e[1], 0xdc, 0x9c);
            o->wb6++;
            break;
        case 6:
            if (((unsigned char *)o->d90)[4] < 2) break;
            o->anim = D_8011F868[0];
            FUN_8001fe6c(o);
            ((unsigned char *)o->d90)[4]++;
            o->wb6++;
            break;
        case 7:
            setEventStarted(e[1], 0, 0);
            o->wb6++;
            break;
        case 8:
            setEventComplete(e[1], 0);
            o->wb6++;
            break;
        case 9:
            D_8009C975 = 3;
            o->wb6++;
            break;
        case 10:
            if (D_8009C975 == 1) o->wb6++;
            break;
        case 11:
            D_8009C975 = 4;
            o->wb6++;
            break;
        case 12:
            if (D_8009C975 == 0) o->wb6++;
            break;
        case 13:
            D_1F8001CC = 1;
            D_1F8001CD = 3;
            D_1F8001C6 = 2;
            FUN_8001f110(0);
            FUN_800171b8(1, FUN_8001d6a4);
            o->wb6++;
            break;
        case 14:
            startAreaBgm();
            o->wb6++;
            break;
        case 15:
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 1;
            o->wb6++;
            break;
        case 16:
            D_800A60EA = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 0;
            o->wb6++;
            break;
        case 17:
            if (PLAYER_H->p.whole > 104) PLAYER_H->p.whole = 104;
            break;
        case 18:
            switch (o->step) {
            case 0:
                o->animFrame = 0;
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->movetab = MOVE_TABLE_0;
                o->step++;
            case 1:
                applyAnimVelocityX(o, 0);
                if (o->a.p.whole < 261) break;
                o->anim = D_8011F868[0];
                D_8009C93B = 1;
                FUN_8001fe6c(o);
                o->w22 = 60;
                o->wb8 = 0;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) {
                    s.x = o->h->p.whole;
                    s.y = o->y.p.whole;
                    s.z = o->d->p.whole;
                    dispatchAreaItemUpdate(o, &s, 1);
                    o->step = 0;
                    o->animFrame = 1;
                    o->wb6++;
                }
                break;
            }
            break;
        case 19:
            switch (o->step) {
            case 0:
                o->animFrame = 1;
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->movetab = MOVE_TABLE_0;
                o->wb8 = 1;
                o->step++;
            case 1:
                applyAnimVelocityX(o, 1);
                if (o->a.p.whole >= 176) break;
                o->anim = D_8011F874[0];
                FUN_8001fe6c(o);
                o->w22 = 60;
                o->wb8 = 2;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) {
                    o->step = 0;
                    o->animFrame = 1;
                    o->wb6++;
                }
                break;
            }
            break;
        case 20:
            FUN_80026c50(0x2d, 1, 1);
            o->wb6++;
            break;
        case 21:
            {
                int v = GAME.playerMaxHealth;
                D_800A60D0 = v;
                D_800A60D2 = v;
                D_8009C970 = v;
            }
            FUN_8001e4f0(10);
            o->wb6++;
            break;
        case 22:
            o->wb6++;
            o->step = 0;
            o->wba = o->wb6;
            break;
        case 23:
            o->wb6 = o->wba;
            break;
        }
        if (FUN_800202b4(o)) FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
