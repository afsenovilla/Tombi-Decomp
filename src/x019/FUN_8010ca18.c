// FUNC 8010ca18 912 X019
// MATCHING 8010ca18 912
#include "TOBJ.H"
typedef struct {
    char pad0[2];
    short w02;
    char pad4[3];
    unsigned char b7;
    unsigned char b8;
    unsigned char b9;
    char pada[2];
    short w0c;
    short w0e;
    char pad10[0x18];
    unsigned short w28;
    unsigned short w2a;
    char pad2c[2];
    unsigned short w2e;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
extern G330 *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned short DAT_8009d670;
extern int DAT_8009c960;
extern unsigned char DAT_8009d2b0;
extern unsigned short DAT_1f8001fc;
extern unsigned short DAT_1f8003c6;
extern char DAT_80011148[];
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8010c7b0(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e560(int, int);
extern void FUN_8010c8d4(TObj *);

void FUN_8010ca18(TObj *o)
{
    volatile unsigned short *pad;
    int t;
    Fix16 *h;
    int v;
    DAT_8009c330->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        o->wb6 = -0x420;
        DAT_8009c330->w0e = 0;
        DAT_8009c330->w02 = 0x10;
        o->wb2 = DAT_8009f0ec->box1;
        OB(o, 0xa9) = 1;
        o->animFrame &= 1;
        DAT_8009c330->b8 = 0;
        DAT_8009c330->b9 = 0;
        DAT_8009c330->w0c = 0;
        DAT_8009c330->w2e = 0xffff;
        DAT_8009c330->w28 = 0xffff;
        DAT_8009c330->w2a = 0xffff;
        o->velH = DAT_8009f0ec->h->p.whole - o->h->p.whole;
        o->velV = DAT_8009f0ec->y.p.whole - o->y.p.whole;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = o->wb8;
        o->h->p.whole = DAT_8009f0ec->h->p.whole + o->wb8;
        o->y.p.whole = DAT_8009f0ec->y.p.whole + o->wba + 8;
        o->anim = DAT_80011148;
        FUN_8001fe94(o, 0);
        o->timer = 10;
        o->state++;
    case 1:
        FUN_8010c7b0(o);
        if (--o->timer == 0)
            o->state = 2;
        break;
    case 2:
        if (DAT_8009f0ec->type == 0x35)
            goto up;
        pad = &DAT_8009d670;
        if (*pad & 0x10) {
            FUN_800eeb5c(o, 0x26);
            FUN_8001fec0(o);
            if (o->b9e != 0xc)
                o->y.raw -= 0x18000;
        } else if (*pad & 0x40) {
        up:
            FUN_800eeb5c(o, 0x27);
            FUN_8001fec0(o);
            o->y.raw += 0x28000;
        }
        if (*(unsigned short *)o->anim == 0xda)
            *(signed char *)&o->b0f = 1;
        else
            *(signed char *)&o->b0f = -8;
        FUN_8010c7b0(o);
        if (DAT_8009c960 == 0x30000)
            break;
        if (o->animFrame & 1)
            t = DAT_1f8001fc & 0x80;
        else
            t = DAT_1f8001fc & 0x20;
        if (t) {
            FUN_8001e560(0x1e, 8);
            DAT_8009c330->w2e = 0xff;
            FUN_800eeb5c(o, 0x38);
            o->state = 3;
        }
        break;
    case 3:
        if (FUN_8001fec0(o)) {
            h = o->h;
            v = h->p.whole;
            if (o->animFrame & 1)
                t = v - 0xc;
            else
                t = v + 0xc;
            h->p.whole = t;
            o->anim = DAT_80011148;
            o->animFrame ^= 1;
            FUN_8001fe94(o, 0);
            o->timer = 10;
            o->state = 1;
        }
        break;
    default:
        return;
    }
    if (DAT_1f8001fc & DAT_1f8003c6) {
        DAT_8009d2b0 = 0;
        FUN_8010c8d4(o);
    }
}
