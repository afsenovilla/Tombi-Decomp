// FUNC 8010d888 644 X000
// MATCHING 8010d888 644
// VERSION SLPS_01144 x00 8010D2D0 872
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj t; char pad[10]; unsigned char bca; } PObj;
extern TObj *PLAYER_EXT;
extern void playerSetAnim(TObj *, int, int);
extern int AnimAdvance(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern short GVARS_PLAYER_DRIFT_H, GVARS_PLAYER_DRIFT_Y;
extern short f1(int, int), f2(int, int);
extern void f3(TObj *), f4(TObj *, int, int), f5(TObj *, int, int, int);
static __inline__ void swimMove(TObj *o)
{
    if (o->wb2 > 0x400) o->wb2 = 0x400;
    if (o->wb2 < 0) o->wb2 = 0;
    o->h->raw += GVARS_PLAYER_DRIFT_H * 0x100;
    o->y.raw += GVARS_PLAYER_DRIFT_Y * 0x100;
    o->velH = f1(o->wb6, o->wb2);
    o->velV = f2(o->wb6, o->wb2);
    o->h->raw += o->velH * 0x100;
    o->y.raw += o->velV * 0x100;
    f3(o);
    f4(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
    f5(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
}

void func_8010D2D0(TObj *o)
{
    unsigned short d;
    switch (o->state) {
    case 0:
        playerSetAnim(o, 0x44, 0);
        o->timer = 10;
        ((PObj *)o)->bca = 1;
        PLAYER_EXT->animFrame = 0xff;
        o->state++;
    case 1:
        if (AnimAdvance(o) && (unsigned short)(*(unsigned short *)o->anim - 0x160) >= 4)
            PlayerSetAnimIfChanged(o, 0x44);
        d = o->w76 & 7;
        switch (d) {
        case 0: o->w7a = 0; break;
        case 1: o->w7a = 0x20; break;
        case 2: o->w7a = 0x40; break;
        case 3: o->w7a = 0x60; break;
        case 4: o->w7a = 0x80; break;
        case 5: o->w7a = 0xa0; break;
        case 6: o->w7a = 0xc0; break;
        case 7: o->w7a = 0xe0; break;
        }
        if (o->w76 & 8)
            o->w74 = 0;
        else
            o->w74 = 0x200;
        if ((d = (o->w7a - o->wb6) & 0xff)) {
            if (d < 0x80)
                o->wb6 = o->wb6 + 4;
            else
                o->wb6 = o->wb6 - 4;
        }
        o->wb6 = (unsigned char)o->wb6;
        if (((o->wb6 - 0x40) & 0xff) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->wb6 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->wb6;
        }
        if ((short)(o->w74 - o->wb2) > 0)
            o->wb2 += 8;
        else
            o->wb2 -= 8;
        if (SPAD.s1fc & SPAD.s3c4) {
            if ((unsigned short)(*(unsigned short *)o->anim - 0x160) < 4) {
                PlayerSetAnimIfChanged(o, 0x45);
                o->wb2 = 0x400;
            }
        }
        swimMove(o);
        if (SPAD.s1fc & SPAD.s3c6) {
            ((PObj *)o)->bca = 0;
            o->step = 0x44;
            o->state = 0;
        }
        if (o->y.p.whole < o->w56 + 8) {
            o->step = 0x3d;
            o->state = 0;
        }
        break;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"
typedef struct { TObj t; char pad[10]; unsigned char bca; } PObj;
extern TObj *PLAYER_EXT;
extern void playerSetAnim(TObj *, int, int);
extern int AnimAdvance(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void playerSwimMove(TObj *);

void playerStepSwimUnderwater(TObj *o)
{
    unsigned short d;
    switch (o->state) {
    case 0:
        playerSetAnim(o, 0x44, 0);
        o->timer = 10;
        ((PObj *)o)->bca = 1;
        PLAYER_EXT->animFrame = 0xff;
        o->state++;
    case 1:
        if (AnimAdvance(o) && (unsigned short)(*(unsigned short *)o->anim - 0x136) >= 4)
            PlayerSetAnimIfChanged(o, 0x44);
        d = o->w76 & 7;
        switch (d) {
        case 0: o->w7a = 0; break;
        case 1: o->w7a = 0x20; break;
        case 2: o->w7a = 0x40; break;
        case 3: o->w7a = 0x60; break;
        case 4: o->w7a = 0x80; break;
        case 5: o->w7a = 0xa0; break;
        case 6: o->w7a = 0xc0; break;
        case 7: o->w7a = 0xe0; break;
        }
        if (o->w76 & 8)
            o->w74 = 0;
        else
            o->w74 = 0x200;
        if ((d = (o->w7a - o->wb6) & 0xff)) {
            if (d < 0x80)
                o->wb6 = o->wb6 + 4;
            else
                o->wb6 = o->wb6 - 4;
        }
        o->wb6 = (unsigned char)o->wb6;
        if (((o->wb6 - 0x40) & 0xff) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->wb6 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->wb6;
        }
        if ((short)(o->w74 - o->wb2) > 0)
            o->wb2 += 8;
        else
            o->wb2 -= 8;
        if (SPAD.s1fc & SPAD.s3c4) {
            if ((unsigned short)(*(unsigned short *)o->anim - 0x136) < 4) {
                PlayerSetAnimIfChanged(o, 0x45);
                o->wb2 = 0x400;
            }
        }
        playerSwimMove(o);
        if (SPAD.s1fc & SPAD.s3c6) {
            ((PObj *)o)->bca = 0;
            o->step = 0x44;
            o->state = 0;
        }
        if (o->y.p.whole < o->w56 + 8) {
            o->step = 0x3d;
            o->state = 0;
        }
        break;
    }
}
#endif
