// FUNC 80131ca4 2548 X001
// MATCHING 80131ca4 2548
#include "TOBJ.H"
typedef struct { short x, y, z, pad; int lim; } SB;
typedef struct { Fix16 x, y, z; } V3;
extern unsigned char D_8009CEB0[];
extern char D_80077CDC[];
extern short D_8013C984[];
extern short D_8007A1F0[], D_8007A5F0[];
extern unsigned short D_1F80016A, D_1F80016E, D_1F800176;
extern short D_1F80017A[];
extern TObj *FUN_800185f8(void);
extern void FUN_80018790(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001e4f0(int);
extern void FUN_80020aec(int);
extern void FUN_8003e3cc(int, int, V3 *, int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern short FUN_80040278(TObj *, short, short);

void func_80131CA4(TObj *o)
{
    SB *sb = (SB *)&o->wb4;
    TObj *p;
    int k;
    short a, s, t;
    V3 v;

    switch (o->b04) {
    case 0:
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box3 = 0x20;
        o->movetab = D_80077CDC;
        o->velY = 0x200;
        o->w98 = 3;
        o->w9a = 3;
        o->box2 = 0x10;
        k = -0x400;
        o->d84 = 0;
        o->d88 = k;
        o->d8c = 0;
        sb->lim = 0;
        o->b6a = 0;
        o->velX = 0;
        o->b69 = 0;
        o->b04++;
        sb->x = o->a.p.whole;
        sb->y = o->y.p.whole;
        sb->z = o->b.p.whole;
        if (D_8009CEB0[0] != 0) {
            o->h->p.whole = 0x6b8;
            o->y.p.whole = -0x14;
            o->active = 3;
            o->b04 = 2;
            o->d94 = 0;
            if (D_8009CEB0[0] == 2) o->step = 5;
            else o->step = 3;
            break;
        }
        p = FUN_800185f8();
        if (p != 0) {
        p->active = 2;
        p->type = 5;
        p->d84 = 0;
        p->d88 = k;
        p->d8c = 0;
        p->b0a = o->b0a;
        p->b1d = o->b1d;
        p->d90 = (int)o;
        o->d94 = (int)p;
        } else o->d94 = 0;
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            if (o->visible) o->step++;
            break;
        case 1:
            if (o->b69) {
                o->b69 = 0;
                o->step++;
                o->w78 = ((o->d38 + 0x800) & 0xfff) >> 4;
                o->velX = D_8013C984[((unsigned)(o->w78 - 0x40) >> 3) & 0xf];
                break;
            }
            o->y.p.whole += 5;
            if (D_1F80017A[0] < o->y.p.whole) goto reset;
            break;
        case 2:
            if (!o->visible) goto reset;
            o->y.p.whole += 5;
            if ((unsigned short)(D_1F80016A - o->h->p.whole + 0x78) < 0xb9) {
                o->b69 = 0;
                o->step++;
            }
            break;
        case 3:
            if (!o->visible) {
                if (o->h->p.whole < 0x594) goto reset;
                o->h->p.whole = 0x640;
                o->y.p.whole = -0xb2;
                goto done;
            }
            if (o->w98 != 0) {
                if (o->h->p.whole < 0x242) {
                    o->velY = -0x300;
                    o->step++;
                    break;
                }
            } else if (o->b6a) {
                if (o->b6a & 1) o->velX -= 0x20;
                else o->velX += 0x20;
            }
            if (o->b69) {
                o->w78 = ((o->d38 + 0x800) & 0xfff) >> 4;
                o->b69 = 0;
                o->velY = 0x100;
                a = D_8013C984[((unsigned)(o->w78 - 0x40) >> 3) & 0xf];
                if (a == 0) {
                    if (o->velX < 0) {
                        a = 4;
                        t = -o->velX;
                    } else {
                        a = -4;
                        t = o->velX;
                    }
                    if (t < 5) {
                        o->velX = 0;
                        a = 0;
                    }
                }
                o->velX += a;
                if (o->velX != 0) {
                    if (o->velX < 0) {
                        if (o->w98 == 0 && o->velX < -0x280) o->velX = -0x280;
                        s = -o->velX;
                        o->d8c += s >> 2;
                        if (sb->lim < o->d8c) {
                            FUN_8001e4f0(0x50);
                            sb->lim = o->d8c + 0x600;
                        }
                    } else {
                        o->d8c -= o->velX >> 2;
                        if (o->w98 == 0 && o->velX > 0x280) o->velX = 0x280;
                        s = -o->velX;
                        if (o->d8c < sb->lim) {
                            FUN_8001e4f0(0x50);
                            sb->lim = o->d8c - 0x600;
                        }
                    }
                    o->velV = s * D_8007A1F0[o->w78] >> 12;
                    o->velH = s * D_8007A5F0[o->w78] >> 12;
                } else {
                    o->velV = 0;
                    o->velH = 0;
                }
            } else {
                o->velY += 0x20;
                if (o->velY > 0x500) o->velY = 0x500;
            }
            o->velV += o->velY;
            o->h->raw += o->velH << 8;
            o->y.raw += o->velV << 8;
            if (o->w98 == 0 && o->h->p.whole < 0x120) {
                o->h->p.whole = 0x11f;
                if (o->velX < 0) o->velX = 0;
            }
            if (o->h->p.whole >= 0x592 && o->y.p.whole >= -0xb8) {
            done:
                o->active = 2;
                o->b04 = 2;
                o->step = 1;
            }
            break;
        case 4:
            if (!o->visible) goto reset;
            FUN_8001fa88(o, 0);
            o->velY += 0x30;
            if (o->velY > 0x500) o->velY = 0x500;
            else if (o->velY > 0) {
                o->b69 = 0;
                o->step++;
            }
            o->y.raw += o->velY << 8;
            break;
        case 5:
            if (!o->visible) {
            reset:
                if ((unsigned short)(D_1F800176 - 0x140) < 0x2f9) break;
                o->b04 = 1;
                o->step = 0;
                o->a.p.whole = o->wb4;
                o->y.p.whole = o->wb6;
                o->b.p.whole = o->wb8;
                break;
            }
            if (o->b69) {
                o->step = 3;
                o->velX = 0x200;
                break;
            }
            FUN_8001fa88(o, 0);
            o->velY += 0x30;
            if (o->velY > 0x500) o->velY = 0x500;
            o->y.raw += o->velY << 8;
            break;
        }
        p = (TObj *)o->d94;
        if (p == 0 || o->w98 != 0) break;
        goto drop;
    case 2:
        switch (o->step) {
        case 0:
            FUN_80020aec(o->b6b);
            v.x.p.whole = o->h->p.whole;
            v.y.p.whole = o->y.p.whole;
            v.z.p.whole = o->d->p.whole;
            FUN_8003e3cc(0, 0, &v, 0x80, -0x400);
            FUN_8003e3cc(2, 1, &v, -0x80, -0x400);
            o->b04++;
            p = (TObj *)o->d94;
            if (p) {
            drop:
                p->b04 = 2;
                p->step = 0;
                o->d94 = 0;
            }
            break;
        case 1:
            FUN_8001e4f0(0x50);
            FUN_8005a8a8(0xb4, 0, 0);
            o->step++;
        case 2:
            FUN_800202b4(o);
            o->d8c -= 8;
            o->h->raw += 0x8000;
            o->y.raw += 0x4000;
            if (o->y.p.whole >= -0x95) {
                o->step++;
                D_8009CEB0[0] = 1;
            }
            break;
        case 3:
            FUN_800202b4(o);
            o->d8c -= 8;
            o->h->raw += 0x8000;
            if (o->h->p.whole > 0x6b8) o->h->p.whole = 0x6b8;
            o->y.raw += 0x4000;
            if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
                o->active = 3;
                o->step++;
            }
            break;
        case 4:
            {
                int r;
                unsigned short t;
                t = D_1F80016A - o->h->p.whole + 0x64;
                if (t >= 0xc9) r = 0;
                else {
                    t = D_1F80016E - o->y.p.whole + 0x50;
                    r = t < 0x79;
                }
                if (r) {
                    D_8009CEB0[0] = 2;
                    FUN_8005a9a4(0xb4, 0);
                    o->step++;
                }
            }
        case 5:
            FUN_800202b4(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
