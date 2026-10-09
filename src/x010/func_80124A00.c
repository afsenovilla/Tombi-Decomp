// FUNC 80124a00 1292 X010
// MATCHING 80124a00 1292
#include "TOBJ.H"

extern TObj D_800A6038;
extern short D_1F80016A[], D_1F80016E;
extern short D_8007A5F0[], D_8007A3F0[];
extern int Rand(void);
extern int AnimAdvance(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern int FUN_800205d8(int, int);
extern int func_80123ECC(TObj *o, unsigned char a);

static __inline__ int turn(TObj *o)
{
    int c, a;
    unsigned char d;

    a = FUN_800205d8(D_1F80016A[0] - o->h->p.whole, D_1F80016E - o->y.p.whole);
    c = o->d38;
    a -= c;
    d = a;

    if (d == 0) return 0;
    if (d < 0x81) {
        if (d >= 2) a = 2;
    } else if (d < 0xff) {
        a = 0xfe;
    }
    o->d38 = c + (a & 0xff);
    o->d38 = *(unsigned char *)&o->d38;
    return 1;
}

static __inline__ int near(TObj *o)
{
    if ((unsigned short)(D_800A6038.d->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(D_800A6038.h->p.whole - o->h->p.whole + 0x64) >= 0xc9) return 0;
    return (unsigned short)(D_800A6038.y.p.whole - o->y.p.whole + 0x50) < 0xa1;
}

void func_80124A00(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 1;
        o->anim = (*(void ***)&o->wa8)[1];
        AnimLoadDuration(o);
        o->velX = Rand() & 3;
        turn(o);
        break;
    case 1:
        AnimAdvance(o);
        {
            short sp = o->velH;
            int c = sp * D_8007A5F0[o->d38];
            int s = sp * D_8007A3F0[o->d38];
            o->h->raw += (short)(c >> 12) << 8;
            o->y.raw += (short)(s >> 12) << 8;
        }
        turn(o);
        if (func_80123ECC(o, o->d38)) {
            o->wac = 2;
            o->anim = (*(void ***)&o->wa8)[2];
            AnimLoadDuration(o);
            o->state = 2;
        } else if (!near(o)) {
            o->step = 0;
            o->state = 1;
            o->timer = Rand() & 0xbf;
            o->wac = 0;
            o->anim = (*(void ***)&o->wa8)[0];
            AnimLoadDuration(o);
        }
        switch (o->velX & 3) {
        case 0:
            o->velH += 8;
            if (o->velH > 0x200) o->velH = 0x200;
            break;
        case 1:
            o->velH -= 8;
            if (o->velH < 0x100) o->velH = 0x100;
            break;
        case 2:
            o->velH += 8;
            if (o->velH > 0x180) o->velH = 0x180;
            break;
        case 3:
            o->velH += 8;
            if (o->velH > 0x280) o->velH = 0x280;
            break;
        }
        break;
    case 2:
        if (AnimAdvance(o)) {
            o->velH = 0x100;
            o->timer = 0x10;
            o->wac = 1;
            o->state++;
            o->d38 = (o->d38 + 0x80) & 0xff;
            o->anim = (*(void ***)&o->wa8)[1];
            AnimLoadDuration(o);
        }
        break;
    case 3:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state = 1;
            o->timer = Rand() & 0xbf;
        }
        {
            short sp = o->velH;
            int c = sp * D_8007A5F0[o->d38];
            int s = sp * D_8007A3F0[o->d38];
            o->h->raw += (short)(c >> 12) << 8;
            o->y.raw += (short)(s >> 12) << 8;
        }
        func_80123ECC(o, o->d38);
        break;
    }
}
