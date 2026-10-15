// FUNC 8011f2b0 1044 X001
// MATCHING 8011f2b0 1044
#include "gcd.h"
#include "gvars.h"
#include "player.h"
/* Debt: empty do {} while (0) after the b04 store in the default arm ends the sched block so the call
   arguments are not scheduled above the store. */
#include "TOBJ.H"
typedef struct { signed char anim, z, ang, rad; } E4;

extern unsigned char D_800A6039A[];
extern unsigned short D_800A6066A[];
extern Fix16 * D_800A6078A[];
extern unsigned short D_800A604EA[];
extern Fix16 * D_800A607CA[];
extern int D_800A60C4A[];
extern unsigned char D_800A6047A[];
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

    o->visible = D_800A6039A[0];
    switch (o->state) {
    case 0:
        c = D_8009C985 & 1;
        if (GVARS.b40) {
            switch (GVARS.b41) {
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
        o->visible = D_800A6039A[0];
        o->animFrame = D_800A6066A[0] & 1;
        o->h->p.whole = D_800A6078A[0]->p.whole + y;
        o->y.p.whole = D_800A604EA[0] + x;
        o->d->p.whole = D_800A607CA[0]->p.whole;
        o->d8c = D_800A60C4A[0];
        o->b0f = D_800A6047A[0] + e->z;
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
        o->h->p.whole = D_800A6078A[0]->p.whole + MulCos(c, e->rad);
        o->y.p.whole = D_800A604EA[0] + MulNegSinScaled(c, e->rad);
        o->d->p.whole = D_800A607CA[0]->p.whole;
        o->d8c = D_800A60C4A[0];
        o->b0f = D_800A6047A[0] + e->z;
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
