// FUNC 8011f2b0 1044 X001
// MATCHING 8011f2b0 1044
// VERSION SLPS_01144 x01 8011E1EC 1024
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "gvars.h"
#include "player.h"
/* Debt: empty do {} while (0) after the b04 store in the default arm ends the sched block so the call
   arguments are not scheduled above the store. */
#include "TOBJ.H"
typedef struct { signed char anim, z, ang, rad; } E4;

extern unsigned char PLAYER_VISIBLE[];
extern unsigned short PLAYER_ANIM_FRAME[];
extern Fix16 * PLAYER_H[];
extern unsigned short PLAYER_Y[];
extern Fix16 * PLAYER_D[];
extern int D_800A60C4A[];
extern unsigned char PLAYER_DRAW_DEPTH[];
extern unsigned char D_8009C985;
extern signed char D_80011EB4[];
extern E4 D_80011E30[];
extern void *D_8013E70C, *D_8013E708;
extern int MulCos(short a, int b);
extern int MulNegSinScaled(short a, int b);
extern void printInfoMessage(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern void removeItemFromInventory(int, int);
extern void addItemToInventory(int, int, int);

void func_8011E1EC(TObj *o)
{
    E4 *e;
    short c;
    int x, y;
    unsigned short u;

    o->visible = PLAYER_VISIBLE[0];
    switch (o->state) {
    case 0:
        c = D_8009C985 & 1;
        if (GVARS.useItemPending) {
            switch (GVARS.useItemId) {
            case 5:
            case 7:
            case 0x7c:
            case 0x97:
            case 0x98:
                c++;
            }
        }
        if (c) {
            printInfoMessage(0xf, 2);
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        e = &D_80011E30[D_80011EB4[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (GCD.bcead == 0) {
            o->b04 = 3;
            break;
        }
        if (GCD.bcead == 1) o->anim = D_8013E70C;
        else o->anim = D_8013E708;
        if (o->animFrame & 1) {
            u = D_800A60C4A[0] + 0x80 - e->ang;
            c = u; c &= 0xff;
        } else {
            u = e->ang + D_800A60C4A[0];
            c = u; c &= 0xff;
        }
        y = MulCos(c, e->rad);
        x = MulNegSinScaled(c, e->rad);
        o->visible = PLAYER_VISIBLE[0];
        o->animFrame = PLAYER_ANIM_FRAME[0] & 1;
        o->h->p.whole = PLAYER_H[0]->p.whole + y;
        o->y.p.whole = PLAYER_Y[0] + x;
        o->d->p.whole = PLAYER_D[0]->p.whole;
        o->d8c = D_800A60C4A[0];
        o->b0f = PLAYER_DRAW_DEPTH[0] + e->z;
        o->category |= 0x80;
        AnimLoadDuration(o);
        break;
    case 1:
        e = &D_80011E30[D_80011EB4[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (o->animFrame & 1) {
            u = D_800A60C4A[0] + 0x80 - e->ang;
            c = u; c &= 0xff;
        } else {
            u = e->ang + D_800A60C4A[0];
            c = u; c &= 0xff;
        }
        o->h->p.whole = PLAYER_H[0]->p.whole + MulCos(c, e->rad);
        o->y.p.whole = PLAYER_Y[0] + MulNegSinScaled(c, e->rad);
        o->d->p.whole = PLAYER_D[0]->p.whole;
        o->d8c = D_800A60C4A[0];
        o->b0f = PLAYER_DRAW_DEPTH[0] + e->z;
        if (AnimAdvance(o)) {
            playSFX(9);
            {
                unsigned char *k = &GCD.bcead;
                int a;
                *k = *k + 1;
                switch (*k) {
                case 2:
                    a = 0xd;
                    goto call;
                case 3:
                    a = 0xe;
                call:
                    printInfoMessage(a, 3);
                    o->state = 0;
                    break;
                default:
                    o->b04 = 3;
                    do { } while (0);
                    removeItemFromInventory(0xa, 1);
                    addItemToInventory(0x12, 1, 1);
                    o->state = 0;
                    break;
                }
            }
        }
        break;
    }
}
#else
#include "gcd.h"
#include "gvars.h"
#include "player.h"
/* Debt: empty do {} while (0) after the b04 store in the default arm ends the sched block so the call
   arguments are not scheduled above the store. */
#include "TOBJ.H"
typedef struct { signed char anim, z, ang, rad; } E4;

extern unsigned char PLAYER_VISIBLE[];
extern unsigned short PLAYER_ANIM_FRAME[];
extern Fix16 * PLAYER_H[];
extern unsigned short PLAYER_Y[];
extern Fix16 * PLAYER_D[];
extern int D_800A60C4A[];
extern unsigned char PLAYER_DRAW_DEPTH[];
extern unsigned char D_8009C985;
extern signed char D_80011EB4[];
extern E4 D_80011E30[];
extern void *D_8013E70C, *D_8013E708;
extern int MulCos(short a, int b);
extern int MulNegSinScaled(short a, int b);
extern void printInfoMessage(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern void removeItemFromInventory(int, int);
extern void addItemToInventory(int, int, int);

void bucketFillWithWater(TObj *o)
{
    E4 *e;
    short c;
    int x, y;
    unsigned short u;

    o->visible = PLAYER_VISIBLE[0];
    switch (o->state) {
    case 0:
        c = D_8009C985 & 1;
        if (GVARS.useItemPending) {
            switch (GVARS.useItemId) {
            case 5:
            case 7:
            case 0xe:
            case 0x7c:
            case 0x97:
            case 0x98:
                c++;
            }
        }
        if (c) {
            printInfoMessage(0xf, 2);
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        e = &D_80011E30[D_80011EB4[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (GCD.bcead == 0) {
            o->b04 = 3;
            break;
        }
        if (GCD.bcead == 1) o->anim = D_8013E70C;
        else o->anim = D_8013E708;
        if (o->animFrame & 1) {
            u = D_800A60C4A[0] + 0x80 - e->ang;
            c = u; c &= 0xff;
        } else {
            u = e->ang + D_800A60C4A[0];
            c = u; c &= 0xff;
        }
        y = MulCos(c, e->rad);
        x = MulNegSinScaled(c, e->rad);
        o->visible = PLAYER_VISIBLE[0];
        o->animFrame = PLAYER_ANIM_FRAME[0] & 1;
        o->h->p.whole = PLAYER_H[0]->p.whole + y;
        o->y.p.whole = PLAYER_Y[0] + x;
        o->d->p.whole = PLAYER_D[0]->p.whole;
        o->d8c = D_800A60C4A[0];
        o->b0f = PLAYER_DRAW_DEPTH[0] + e->z;
        o->category |= 0x80;
        AnimLoadDuration(o);
        break;
    case 1:
        e = &D_80011E30[D_80011EB4[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (o->animFrame & 1) {
            u = D_800A60C4A[0] + 0x80 - e->ang;
            c = u; c &= 0xff;
        } else {
            u = e->ang + D_800A60C4A[0];
            c = u; c &= 0xff;
        }
        o->h->p.whole = PLAYER_H[0]->p.whole + MulCos(c, e->rad);
        o->y.p.whole = PLAYER_Y[0] + MulNegSinScaled(c, e->rad);
        o->d->p.whole = PLAYER_D[0]->p.whole;
        o->d8c = D_800A60C4A[0];
        o->b0f = PLAYER_DRAW_DEPTH[0] + e->z;
        if (AnimAdvance(o)) {
            playSFX(9);
            {
                unsigned char *k = &GCD.bcead;
                int a;
                *k = *k + 1;
                switch (*k) {
                case 2:
                    a = 0xd;
                    goto call;
                case 3:
                    a = 0xe;
                call:
                    printInfoMessage(a, 3);
                    o->state = 0;
                    break;
                default:
                    o->b04 = 3;
                    do { } while (0);
                    removeItemFromInventory(0xa, 1);
                    addItemToInventory(0x12, 1, 1);
                    o->state = 0;
                    break;
                }
            }
        }
        break;
    }
}
#endif
