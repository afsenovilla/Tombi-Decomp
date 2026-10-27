// FUNC 8012736c 2060 X010
// MATCHING 8012736c 2060
#include "gcd.h"
#include "gvars.h"
/* Matched. Keep: function-scope `int y` (global pseudo -> v1 in cases 2 and 5) and function-scope `int hv`
   shared by case 2's two scratchpad loads and case 5's o->h pointer (global pseudo in a1; a case-8 use would put
   the pointer in a1 where the game has v0, and block-local it steals v1 from y). Case 2 state++ through y so the
   y load gets an anti-dep and stays after the sb. Case 5: first state++ through a multi-set local s, `y = h - y`,
   h load after sb via hv, `k = ...; k = k >> 15;` (multi-set, subu not birthing-boosted); 2nd state++ as a
   block-local `unsigned char s2 = o->state + 1` placed between `k = K - h` and `k >>= 15`: the lbu temp is
   single-set (boosted, lands next to the addiu), the addiu writes a subreg (never boosted) so sched1 orders it by
   luid (after subu, before sra) while `o->state = s2` written last keeps sb after sh; local-alloc ties lbu temp
   and s2 into one v1 qty spanning the anims-pointer load, which then takes a0. */
#include "TOBJ.H"

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

extern TObj PLAYER;
extern unsigned short *PLAYER_ANIM;
extern unsigned short PLAYER_ANIM_FRAME;
extern short PLAYER_X[];
extern short PLAYER_Y[];
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern short PLAYER_H_WHOLE[];
extern short PLAYER_Y_WHOLE[];
extern int PLAYER_H_COPY;
extern int PLAYER_Y_COPY;
extern int D_1F800168A[];
extern int D_1F80016CA[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern void ObjSetFacingToPlayer(TObj *);
extern void playerSetAnim(TObj *, int, int);
extern void startPadVibration(int, int, int, int);
extern void swimTeacherFloatPlayer(TObj *, int);
extern short TileCollideAt(TObj *, short, short);
extern TObj *showMessageBoxDirect(int, int, void *);
extern void setEventStarted(int, int, int);
extern void addItemToInventory(int, int, int);

#define SETANIM(k) o->anim = ANIMS(o)[k]; AnimLoadDuration(o);
#define WAITCHILD() \
    AnimAdvance(o); \
    if (((TObj *)o->d90)->b04 == 2) { ((TObj *)o->d90)->b04 = 3; o->state++; }

void swimTeacherIntroTalk(TObj *o)
{
    int y;
    int hv;
    switch (o->state) {
    case 0:
        o->b0f = 2;
        SETANIM(0);
        o->state++;
    case 1:
        if (PLAYER_H_WHOLE[0] >= 0xc81 && PLAYER_Y_WHOLE[0] >= -0x27) {
            *(signed char *)&o->b0f = -7;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            PLAYER.active = 5;
            PLAYER_STATE = 5;
            PLAYER_SUBSTATE = 100;
            PLAYER_STEP = 0;
            playerSetAnim(&PLAYER, 0x1e, 0);
            PLAYER_Y[0] = -0x1e;
            o->d34 = -0x1e0000;
            o->timer = 0x3c;
            o->state++;
            playSFX(0xe);
        }
        break;
    case 2:
        if (*PLAYER_ANIM == 0x3d) startPadVibration(0, 0, 0xff, 4);
        if (--o->timer == -1) {
            hv = D_1F800168A[0];
            o->velH = (hv - o->h->raw) >> 15;
            hv = D_1F80016CA[0];
            y = o->state;
            y = y + 1;
            o->state = y;
            y = o->y.raw;
            y = hv - y;
            o->velV = y >> 15;
            o->velY = -0x300;
            o->timer = 0x80;
            *(signed char *)&o->b0f = -9;
            ObjSetFacingToPlayer(o);
            SETANIM(7);
        } else if (o->timer < 0x1e) {
            swimTeacherFloatPlayer(o, 1);
        }
        break;
    case 3:
        swimTeacherFloatPlayer(o, 1);
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velY += 0x10;
        o->y.raw += (short)o->velY << 8;
        if (o->velY > 0) {
            o->state++;
            SETANIM(8);
        }
        o->timer--;
        break;
    case 4:
        swimTeacherFloatPlayer(o, 1);
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (o->velY < 0x300) {
            o->velY += 0x10;
            o->y.raw += (short)o->velY << 8;
        }
        if (o->timer <= 0) {
            o->state++;
            o->a.p.whole = PLAYER_H_WHOLE[0] + 8;
            o->y.p.whole = PLAYER_Y_WHOLE[0];
            o->timer = 8;
        } else {
            o->timer--;
        }
        break;
    case 5:
        swimTeacherFloatPlayer(o, 1);
        if (--o->timer == -1) {
            { int h, s, k; unsigned char s2;
            s = o->state;
            s = s + 1;
            o->state = s;
            y = o->y.raw;
            h = -0x780000;
            y = h - y;
            hv = (int)&o->h->raw;
            h = *(int *)hv;
            o->velV = y >> 15;
            o->velY = -0x500;
            o->timer = 0x80;
            o->animFrame = 1;
            k = 0x0c4e0000 - h;
            s2 = o->state + 1;
            k = k >> 15;
            o->velH = k;
            o->state = s2; }
            SETANIM(7);
        }
        break;
    case 6:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velY += 0x18;
        o->y.raw += (short)o->velY << 8;
        PLAYER_X[0] = o->a.p.whole - 8;
        PLAYER_Y[0] = o->y.p.whole;
        if (o->velY > 0) {
            o->state++;
            SETANIM(8);
        }
        o->timer--;
        break;
    case 7:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->velY += 0x18;
        o->y.raw += (short)o->velY << 8;
        PLAYER_X[0] = o->a.p.whole - 8;
        PLAYER_Y[0] = o->y.p.whole;
        if (o->velY > 0x400) { o->state++; break; }
        break;
    case 8:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velY << 8;
        {
            short *k = PLAYER_X;
            *k = o->a.p.whole - 8;
            PLAYER_Y[0] = o->y.p.whole;
            if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
                o->state++;
                SETANIM(9);
                o->timer = 10;
                PLAYER_ANIM_FRAME = 0;
                PLAYER_STATE = 5;
                PLAYER_SUBSTATE = 0;
                PLAYER_STEP = 0;
                *k -= 4;
            }
        }
        break;
    case 9:
        if (--o->timer == -1) {
            short *k = PLAYER_X;
            *k -= 4;
            o->state++;
            ObjSetFacingToPlayer(o);
            SETANIM(4);
        }
        break;
    case 10:
        if (AnimAdvance(o)) {
            short *k = PLAYER_X;
            *k -= 2;
            *(signed char *)&o->b0f = -7;
            o->state++;
        break;
        }
        break;
    case 11:
        SETANIM(5);
        {
            short *k = PLAYER_X;
            *k -= 2;
        }
        ObjSetFacingToPlayer(o);
        o->state++;
        o->d90 = (int)showMessageBoxDirect(2, 0x13, &o->a);
        break;
    case 12:
    case 14:
    case 18:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            o->state++;
        break;
        }
        break;
    case 13:
        o->d90 = (int)showMessageBoxDirect(2, 0x14, &o->a);
        o->state++;
        break;
    case 15:
        setEventStarted(0x40, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 17:
        o->d90 = (int)showMessageBoxDirect(2, 0x15, &o->a);
        o->state++;
        break;
    case 19:
        addItemToInventory(0x9f, 1, 1);
        o->timer = 0x50;
        o->state++;
        break;
    case 16:
    case 20:
        if (--o->timer == -1) {
            o->state++;
        }
        break;
    case 21:
        GCD.bd078 = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        PLAYER.active = 1;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->state = 0;
        o->step++;
        break;
    }
}
