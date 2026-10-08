// FUNC 8010d390 1120 X008
// MATCHING 8010d390 1120
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    char pad0[8];
    unsigned char b8;
    char pad9[0x2c - 9];
    unsigned short w2c;
    unsigned short w2e;
} G330;
extern G330 *DAT_8009c330;
extern unsigned char DAT_8009ce3d;
extern unsigned char DAT_8009cffb;
extern unsigned short DAT_8009c960, DAT_8009c962;
extern unsigned short DAT_8009d670;
extern unsigned short D_1f8001fc, D_1f8003c4, D_1f8003c6;
extern void FUN_8010cda8(TObj *);
extern void FUN_800eee90(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8010d280(TObj *);
extern void FUN_800eea7c(TObj *, int, int);

void FUN_8010d390(TObj *o)
{
    unsigned int d;
    unsigned short dd;
    short flag;
    short w;
    unsigned short h;

    FUN_8010cda8(o);
    switch (o->state) {
    case 0:
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        o->ba7 = 0;
        U8(o, 0xc3) = 0;
        o->d8c = 0;
        o->wb0 = 0;
        DAT_8009c330->b8 = 0;
        U8(o, 0xca) = 1;
        o->timer = 10;
        FUN_800eee90(o);
        o->ba4 = 0;
        o->b69 = 0;
        if (o->animFrame & 1) {
            o->w76 = 4;
            o->wb6 = 0x80;
        } else {
            o->w76 = 0;
            o->wb6 = 0;
        }
        o->wb2 = 0;
        FUN_800eeb5c(o, 0x43);
        o->state++;
    case 1:
        break;
    default:
        return;
    }
    if (FUN_8001fec0(o)) {
        DAT_8009c330->w2c = 0x47;
        if (DAT_8009c330->w2e != 0x47)
            FUN_800eeb5c(o, 0x47);
    }
    switch (o->w76 & 7) {
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
    d = ((unsigned short)o->w7a - (unsigned short)o->wb6) & 0xff;
    dd = d;
    if (d) {
        if (dd < 0x80)
            o->wb6 += 4;
        else
            o->wb6 -= 4;
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
    if (D_1f8001fc & D_1f8003c4) {
        DAT_8009c330->w2c = 0x43;
        if (DAT_8009c330->w2e != 0x43) {
            FUN_800eeb5c(o, 0x43);
            o->wb2 = 0x400;
        }
    }
    FUN_8010d280(o);
    if (o->y.p.whole < o->w56)
        o->y.p.whole = o->w56;
    w = o->w56;
    if (w + 0x10 < o->y.p.whole) {
        if (DAT_8009ce3d == 0xff || DAT_8009cffb) {
            flag = 0;
            if (DAT_8009c960 == 10 && (DAT_8009c962 == 1 || DAT_8009c962 == 5)) {
                o->y.p.whole = w + 0x10;
                flag = 1;
            }
            if (!flag) {
                FUN_800eea7c(o, 0x44, 0);
                o->step = 0x3e;
                o->state = 0;
            }
        } else {
            o->y.p.whole = w + 0x10;
        }
    }
    if (D_1f8001fc & D_1f8003c6) {
        U8(o, 0xca) = 0;
        flag = 0;
        if ((*(volatile unsigned short *)&DAT_8009d670 & 0x10) && DAT_8009c960 == 10
            && (DAT_8009c962 == 1 || DAT_8009c962 == 5) && o->d->p.whole == 0 && o->y.p.whole < -0xe3) {
            h = o->h->p.whole;
            if ((unsigned short)(h - 0x1cd) < 0x20)
                flag = 1;
            if ((unsigned short)(h - 0x33f) < 0x20)
                flag++;
        }
        if (flag) {
            U8(o, 0xa1) = 0;
            o->step = 0x11;
        } else {
            FUN_800eea7c(o, 4, 0);
            DAT_8009c330->w2e = 0xff;
            o->step = 0x43;
        }
        o->state = 0;
    }
}
