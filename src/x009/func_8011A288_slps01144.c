// JP-only (x09), written from the disassembly
// VERSION SLPS_01144 x09 8011A288 888
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { signed char anim, z, ang, rad; } E4;

extern unsigned char D_800A7B79[];
extern unsigned short D_800A7BA6[];
extern Fix16 * PLAYER_H[];
extern unsigned short PLAYER_Y[];
extern Fix16 * D_800A7BBC[];
extern int D_800A7C04[];
extern unsigned char D_800A7B87[];
extern signed char D_80011BB0[];
extern E4 D_80011B68[];
extern void *D_8012ADA0;
extern int MulCos(short a, int b);
extern int MulNegSinScaled(short a, int b);
extern void printInfoMessage(int, int);
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern void removeItemFromInventory(int, int);
extern void addItemToInventory(int, int, int);
extern void func_8001C988(int, int);
extern void bucketThrowWater(TObj *);

void func_8011A288_slps01144(TObj *o)
{
    E4 *e;
    short c;
    int x, y;
    unsigned short u;

    o->visible = D_800A7B79[0];
    switch (o->state) {
    case 0:
        c = 0;
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
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        e = &D_80011B68[D_80011BB0[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (GCD.bcead == 0) {
            o->b04 = 3;
            break;
        }
        o->anim = D_8012ADA0;
        if (o->animFrame & 1) {
            u = D_800A7C04[0] + 0x80 - e->ang;
            c = u; c &= 0xff;
        } else {
            u = e->ang + D_800A7C04[0];
            c = u; c &= 0xff;
        }
        y = MulCos(c, e->rad);
        x = MulNegSinScaled(c, e->rad);
        o->visible = D_800A7B79[0];
        o->animFrame = D_800A7BA6[0] & 1;
        o->h->p.whole = PLAYER_H[0]->p.whole + y;
        o->y.p.whole = PLAYER_Y[0] + x;
        o->d->p.whole = D_800A7BBC[0]->p.whole;
        o->d8c = D_800A7C04[0];
        o->b0f = D_800A7B87[0] + e->z;
        o->category |= 0x80;
        AnimLoadDuration(o);
        break;
    case 1:
        e = &D_80011B68[D_80011BB0[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (o->animFrame & 1) {
            u = D_800A7C04[0] + 0x80 - e->ang;
            c = u; c &= 0xff;
        } else {
            u = e->ang + D_800A7C04[0];
            c = u; c &= 0xff;
        }
        o->h->p.whole = PLAYER_H[0]->p.whole + MulCos(c, e->rad);
        o->y.p.whole = PLAYER_Y[0] + MulNegSinScaled(c, e->rad);
        o->d->p.whole = D_800A7BBC[0]->p.whole;
        o->d8c = D_800A7C04[0];
        o->b0f = D_800A7B87[0] + e->z;
        playSFX(9);
        o->b04 = 3;
        removeItemFromInventory(12, 1);
        addItemToInventory(13, 1, 1);
        func_8001C988(40, 1);
        o->state = 3;
        break;
    }
}
#endif
