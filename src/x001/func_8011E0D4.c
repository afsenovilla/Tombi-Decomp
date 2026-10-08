// FUNC 8011e0d4 824 X001
// MATCHING 8011e0d4 824
#include "TOBJ.H"
#define VX (*(unsigned short *)&o->velX)
extern int FUN_80020078(TObj *, int);
extern void FUN_80018838(TObj *);
extern unsigned char D_8009C942;
extern unsigned short D_8007A3F0[];
extern short D_8013C6D0[];
extern short D_8013C6FC[];
extern short D_8013C714[];

static __inline__ short wave(TObj *o, int vx, short tm, int vy)
{
    int h;
    vx = (vx + 0x10) & 0xff;
    o->velX = vx;
    h = D_8007A3F0[vx];
    o->timer = ++tm;
    o->velH = h;
    o->d8c = ((short)h >> vy) & 0xfff;
    return tm;
}
static __inline__ short big(short a, short b)
{
    if (b > a) return b + 0x20;
    return a + 0x20;
}
static __inline__ void setup(TObj *q, int y, int d)
{
    q->d84 = 0;
    q->d88 = 0;
    q->d8c = 0;
    q->b68 = 0;
    q->d34 = y;
    q->active = 3;
    q->b0f = 4;
    q->d38 = d;
}

void func_8011E0D4(TObj *o)
{
    short *p;
    short x;
    int v;
    short t;
    int y;

    switch (o->b04) {
    case 0:
        o->box0 = 0xc;
        o->box1 = 0x18;
        p = &D_8013C6D0[o->subtype * 2]; o->box2 = *p++;
        o->box3 = *p;
        o->b04++;
        setup(o, o->y.p.whole - 0x18, D_8013C6FC[o->subtype]);
        break;
    case 1:
        x = big(o->box2, o->box0);
        if (D_8009C942) {
            FUN_80020078(o, x);
            break;
        }
        FUN_80020078(o, x);
        if (o->step == 0) {
            if (o->active == 1) o->active = 3;
            if (o->d94 == 0) {
                o->active = 1;
                o->step++;
            }
        }
        switch (o->state) {
        case 0:
            if (o->b68) {
                o->velX = 0x80;
                if (o->animFrame & 1) o->velX = 0;
                o->b68 = 0;
                o->timer = 0;
                o->state++;
                o->velY = D_8013C714[0];
            }
            break;
        case 1:
            if (o->b68) {
                if (o->animFrame & 1) {
                    v = VX;
                    if ((unsigned char)(v - 0x40) < 0x80)
                        o->velX = (unsigned char)(~v - 0x7f);
                } else {
                    v = VX;
                    if ((unsigned char)(v - 0x40) > 0x80)
                        o->velX = (unsigned char)(~v - 0x7f);
                }
                o->b68 = 0;
                o->timer = (((unsigned short)o->velX >> 4) & 0xf) - 0xf;
            }
            t = wave(o, VX, o->timer, o->velY);
            if (t == 0x30) {
                o->d8c = 0;
                o->state--;
            }
            if (o->timer < 0) o->velY = D_8013C714[0];
            else o->velY = D_8013C714[(o->timer >> 3) & 0xf];
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
