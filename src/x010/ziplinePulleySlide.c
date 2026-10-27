// FUNC 801186e0 920 X010
// MATCHING 801186e0 920
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
extern void playSFX(int);
extern short probeFloorPlaneAt(TObj *, short, short);

void ziplinePulleySlide(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b6a == 0) {
            break;
        }
        switch (o->b0c) {
        case 5:
        case 7:
            o->w74 = 0x300;
            o->w76 = 0x300;
            o->velX = 0x18;
            o->velY = 0x18;
            o->velH = 0;
            o->velV = 0;
            o->step++;
            playSFX(0xa2);
            break;
        case 6:
            o->w74 = 0x300;
            o->w76 = 0x260;
            o->velX = 0x18;
            o->velY = 0x13;
            o->velH = 0;
            o->velV = 0;
            o->step++;
            playSFX(0xa2);
            break;
        case 8:
            o->w74 = 0x400;
            o->w76 = 0x180;
            o->velX = 0x20;
            o->velY = 0xb;
            o->velH = 0;
            o->velV = 0;
            o->step++;
            playSFX(0xa2);
            break;
        }
        break;
    case 1:
        o->velH += o->velX;
        o->velV += o->velY;
        if (o->velH >= o->w74) {
            o->velH = o->w74;
            o->step++;
        }
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        break;
    case 2:
        if (GV2.bc3 & 0x20) {
            if (o->b0c == 8) {
                if (o->a.p.whole >= 0xa6f) {
                    o->y.raw += o->velV << 8;
                    if (probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole + 8)) {
                        o->step++;
                    }
                    break;
                }
                o->h->raw += o->velH << 8;
                o->y.raw += o->velV << 8;
                break;
            }
        } else {
            if (o->b0c == 8) {
                if (o->a.p.whole >= 0xac9) {
                    if (o->b6a) {
                        unsigned char *c = &GVARS.b58;
                        GCD.bcf06 = 0;
                        GVARS.b3f = 1;
                        GVARS.hurtState = 1;
                        GVARS.b3e = 1;
                        *c = *c + 1;
                    }
                    o->active = 2;
                    o->step++;
                    break;
                }
                o->h->raw += o->velH << 8;
                o->y.raw += o->velV << 8;
                break;
            }
        }
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole + 8)) {
            o->active = 2;
            o->step++;
        }
        break;
    case 3:
        if (o->b0c == 8 && o->b6a == 0) {
            o->active = 2;
        }
        if (o->visible == 0) {
            o->step++;
            o->h->raw = o->d30;
            o->y.raw = o->d34;
            if (o->b0c == 8) {
                GVARS.b58 = 0;
            }
        }
        break;
    case 4:
        if (o->visible == 0) {
            o->step = 0;
            o->active = 1;
        } else {
            o->visible = 0;
        }
        break;
    }
}
