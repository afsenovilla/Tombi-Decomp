// FUNC 80108708 968 X010
// MATCHING 80108708 968
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
extern TObj *D_8009C330;
extern unsigned short D_1f8001c8;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void AnimAdvance(TObj *);
extern void FUN_800ee428(TObj *);
extern short probeFlaggedSegmentB(int, int, int);
void func_80108708(TObj *o)
{
    unsigned short f;
    short r;
    switch (o->state) {
    case 0:
        (*(unsigned char*)&D_8009C330->w08) = o->active;
        f = o->animFrame;
        o->velX = 0x5a;
        o->velY = -0x200;
        o->active = 2;
        o->d8c = 0;
        o->b9c = 0;
        (*(unsigned char*)&o->wac) = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        (*(unsigned char*)((char*)&o->wac + 1)) = 0;
        o->b69 = 0;
        (*(signed char*)&o->b0f) = -20;
        (*(unsigned char*)((char*)&o->da0 + 2)) = 2;
        o->animFrame = f & 1;
        PlayerSetAnimIfChanged(o, 0x15);
        o->state++;
        if (GAME.selectedArea == 10 && (GAME.selectedSection == 1 || GAME.selectedSection == 5)) {
            o->velY = -0x300;
            o->state = 2;
        }
    case 1:
        AnimAdvance(o);
        if (D_1f8001c8 & 1) {
            o->d->p.whole = o->d->p.whole - 5;
        } else {
            o->d->p.whole = o->d->p.whole + 5;
        }
        o->y.raw += o->velY << 8;
        o->velX -= 5;
        o->velY += 0x10;
        if (o->velX == 0) {
            (*(signed char*)&o->b0f) = -8;
            PlayerSetAnimIfChanged(o, 0x16);
            o->d->p.whole = (short)(o->d->p.whole / 90) * 90;
            if (S16(o, 0xe0) != 0) {
                o->active = 3;
            } else {
                o->active = 1;
            }
            o->b9c = 2;
            o->wb2 = 0;
            D_8009C330->timer = 0;
            FUN_800ee428(o);
            o->b04 = 1;
            (*(unsigned char*)((char*)&o->da0 + 2)) = 0;
            o->step = 2;
            o->state = 3;
        } else {
            r = probeFlaggedSegmentB(o->h->p.whole, o->y.p.whole,
                             (short)(o->d->p.whole + ((D_1f8001c8 & 1) ? -5 : 5)));
            if (r != 0) {
                (*(unsigned char*)&o->waa) = 1;
                if (r == 1) {
                    o->d->p.whole += (D_1f8001c8 & 1) ? -5 : 5;
                    o->d->p.whole = (short)(o->d->p.whole / 90) * 90;
                    (*(unsigned char*)((char*)&o->da0 + 2)) = 3;
                    o->wb2 = 0;
                    o->step = 0x15;
                    o->state = 0;
                }
            }
        }
        break;
    case 2:
        AnimAdvance(o);
        if (D_1f8001c8 & 1) {
            o->d->p.whole = o->d->p.whole - 5;
        } else {
            o->d->p.whole = o->d->p.whole + 5;
        }
        o->y.raw += o->velY << 8;
        o->velY += 0x10;
        if (o->velY > 0x680) {
            o->velY = 0x680;
        }
        break;
    }
}
