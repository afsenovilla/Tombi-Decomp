// FUNC 801176b0 1836 X006
// MATCHING 801176b0 1836
#include "TOBJ.H"
extern TObj *D_8009C94C;
extern char *D_1F8001D4;
extern unsigned char D_8009C958[];
extern unsigned char D_8009C93A, D_8009CFCE, D_8009C93E, D_8009C942;
extern unsigned char D_8009C970, D_8009C93C, D_8009C975, D_8009C967, D_8009C938;
extern unsigned char D_8009E375, D_8009E376, D_8009E377, D_8009F085, D_8009F086, D_8009F087;
extern short D_1F80016A;
extern int D_1F8002D0[];
extern unsigned short D_1F8001FC;
extern short D_8009CD96, D_8009CDA0, D_8009CD94, D_8009C982, D_8009D2AC, D_8009D2A8, D_8009D2AA;
extern unsigned short D_8009C960, D_8009C962;
extern void *D_80122FB0, *D_80122FB4[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_80020078(TObj *, int);
extern void func_80118304(int, int, int);
extern int FUN_8001e4f0(int);
extern void FUN_8001eaa4(int);
extern void FUN_8001f110(int);
extern void FUN_8004d620(int, int);
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_8001e560(int, int);
extern TObj *FUN_8002dcc8(int, int, Fix16 *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8001f2ec(int);
extern void FUN_800187e4(TObj *);

void func_801176B0(TObj *o)
{
    TObj *pl = D_8009C94C;
    char *p;
    unsigned short a, b, k;
    unsigned char *c;

    switch (o->b04) {
    case 0:
        o->b.p.whole = 0x1f00;
        o->w1e = 3;
        o->b0f = 10;
        o->animFrame = 1;
        o->b0d = 0;
        o->b0a = 0;
        o->b0c = 3;
        o->b04++;
        o->d3c = D_1F8002D0[0];
        o->anim = D_80122FB0;
        AnimLoadDuration(o);
        D_8009C93E = 1;
        D_8009C958[0] = 1;
        o->d30 = FUN_8001e4f0(0xfd);
        break;
    case 1:
        FUN_80020078(o, 0xa0);
        switch (o->step) {
        case 0:
            if (D_8009C93A == 0)
                break;
            if (D_1F80016A < 0x177)
                break;
            D_8009C958[0] = 0;
            FUN_8001eaa4(o->d30);
            FUN_8001e4f0(0xfe);
            o->timer = 0x1e;
            o->step++;
            if (D_8009CFCE == 1)
                o->step = 5;
            break;
        case 1:
        case 6:
            if (--o->timer != -1)
                break;
            o->step++;
            break;
        case 2:
            o->step++;
            o->d90 = (int)FUN_8002dcc8(2, 0, &o->a);
            D_8009CFCE = 1;
            o->anim = D_80122FB4[0];
            AnimLoadDuration(o);
            break;
        case 3:
        case 8:
            AnimAdvance(o);
            if (((TObj *)o->d90)->b04 != 2)
                break;
            ((TObj *)o->d90)->b04 = 3;
            o->step++;
            break;
        case 4:
            FUN_8005a8a8(0x79, 0, 0);
            o->step = 6;
            o->timer = 0x15e;
            break;
        case 5:
            o->timer = 0x28;
            o->step++;
            break;
        case 7:
            o->step++;
            o->d90 = (int)FUN_8002dcc8(2, 1, &o->a);
            o->anim = D_80122FB4[0];
            AnimLoadDuration(o);
            break;
        case 9:
            o->step++;
            o->anim = D_80122FB0;
            AnimLoadDuration(o);
            break;
        case 10:
            func_80118304(o->b0c, 0xa0, 0x50);
            FUN_8001e4f0(o->b0c == 0 ? 0x100 : 0xff);
            o->timer = 0x3c;
            o->step++;
            break;
        case 11:
            if (--o->timer != 0)
                break;
            if (o->b0c == 0) {
                FUN_8001e4f0(0xfc);
                o->step++;
            } else {
                o->b0c--;
                o->step--;
            }
            break;
        case 12:
            D_8009C958[0] = 4;
            o->timer = 0x14;
            o->step++;
            break;
        case 13:
            if (--o->timer != -1)
                break;
            o->step++;
            o->d30 = FUN_8001e4f0(0xfd);
            break;
        case 14:
            o->step = 0;
            o->b04++;
            break;
        }
        break;
    case 2:
        FUN_80020078(o, 0xa0);
        switch (o->step) {
        case 0:
            if (pl->animTimer > 0xc0) {
                func_80118304(4, 0x64, 0x78);
                o->b04 = 3;
                break;
            }
            if (D_8009C942 != 1)
                break;
            FUN_8001eaa4(o->d30);
            FUN_8001e4f0(0xfe);
            o->step++;
            break;
        case 1:
            FUN_8001f110(1);
            FUN_8001e4f0(0x101);
            FUN_8004d620(0x25, 2);
            c = &D_8009C970;
            if (--*c == 0) {
                o->step = 8;
                o->timer = 0x3c;
            } else {
                o->timer = 0x3c;
                o->step++;
            }
            break;
        case 2:
            if (--o->timer != -1)
                break;
            o->step++;
            FUN_8002dc50(2, 0xb, 0xa0, 0x6e);
            o->timer = 0x46;
            break;
        case 3:
            if (--o->timer != -1)
                break;
            o->step++;
            break;
        case 4:
            k = D_1F8001FC;
            if (k & 0x4000) {
                o->step = 6;
                FUN_8001e560(10, 10);
            } else if (k & 0x2000) {
                FUN_8001e560(0x1b, 0x1c);
                o->step++;
                D_8009CD96 = 4;
                D_8009CDA0 = 1;
                D_8009CD94 = 0;
                D_8009C93C = 0;
                D_8009C975 = 3;
            }
            break;
        case 5:
            if (D_8009C975 != 1)
                break;
            p = D_1F8001D4;
            *(short *)(p + 0x4c) = 7;
            *(short *)(p + 0x4e) = 0;
            break;
        case 6:
            FUN_8001f110(1);
            D_8009C93C = 0;
            D_8009C975 = 3;
            o->step++;
            break;
        case 7:
            if (D_8009C975 != 1)
                break;
            p = D_1F8001D4;
            *(short *)(p + 0x4c) = 0;
            *(short *)(p + 0x4e) = 5;
            a = D_8009C960;
            b = D_8009C962;
            D_8009C982 = 0;
            D_8009CDA0 = 0;
            D_8009D2AC = 0;
            D_8009C967 = 0;
            D_8009E375 = 0;
            D_8009E376 = 0;
            D_8009E377 = 0;
            D_8009F085 = 0;
            D_8009F086 = 0;
            D_8009F087 = 0;
            D_8009D2A8 = a;
            D_8009D2AA = b;
            break;
        case 8:
            if (--o->timer != -1)
                break;
            FUN_8001f2ec(3);
            o->timer = 0x5a;
            o->step++;
            break;
        case 9:
            if (--o->timer != -1)
                break;
            D_8009C938 = 2;
            break;
        }
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
