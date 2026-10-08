// FUNC 8002b3c8 1276 MAIN0
/* score 30 (ncheck): only case 1 differs: the two /30 divisions (mult magic) get a1/a2/a3 instead of game a2/a1/t0 and the sign term (x>>31) is scheduled later. Tried int/short temps, statement orders, *256 vs <<8. Tail: (o->d30 >> 16) gives lh; order a, y, b. */
#include "TOBJ.H"
typedef struct { TObj t; short wc0, wc2, wc4, wc6; unsigned short wc8, wca, wcc; } SX;
#define X(o) ((SX *)(o))
#define HI(f) (*(short *)((char *)&(f) + 2))
extern unsigned char D_800B0BBC[], D_800B0BBD[], D_800B0BBE[], D_800B0BBF[];
extern unsigned short FUN_8001f9e0(void);
extern short FUN_8001fdac(int, int);

void FUN_8002b3c8(TObj *o)
{
    int off, x, y, x2, y2, c0, c1, c2, c3;

    switch (o->step) {
    case 0:
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->step++;
        break;
    case 1:
        if (--o->timer != 0) break;
        if (o->active == 1) {
            int t = o->a.p.whole - 0xa0; int u;
            o->velH = ((X(o)->wc4 - t) << 8) / 30;
            u = o->y.p.whole + 0x78;
            o->velV = ((X(o)->wc6 - u) << 8) / 30;
        } else {
            o->velH = 0;
            o->velV = 0;
        }
        o->velY = -0x800;
        o->timer = 30;
        o->step++;
        break;
    case 2:
        o->d30 += o->velH << 8;
        o->d34 += o->velV << 8;
        o->d34 += o->velY << 8;
        o->velY += 0x88;
        if (--o->timer != 0) break;
        if (o->subtype == 0)
            o->timer = 0xb4;
        else
            o->timer = 0x5a;
        o->wb4 = 0;
        o->wb6 = (FUN_8001f9e0() & 3) + 3;
        o->wb8 = 0;
        o->wba = (FUN_8001f9e0() & 7) + 3;
        o->step++;
        break;
    case 3:
        if (o->subtype == 0) {
            o->d34 = ((unsigned short)X(o)->wc6 + FUN_8001fdac(o->wb4, 4)) << 16;
        }
        o->d8c = FUN_8001fdac(o->wb8, 0x80);
        o->wb4 += o->wb6;
        o->wb8 += o->wba;
        o->wb4 = (unsigned char)o->wb4;
        o->wb8 = (unsigned char)o->wb8;
        if (--o->timer != 0) break;
        o->d8c = 0;
        if (o->subtype == 0) {
            o->step++;
            break;
        }
        o->timer = 1;
        o->step = 5;
        break;
    case 4:
        o->d30 += 0x80000;
        if (o->d30 > 0x1400000) o->b04++;
        break;
    case 5:
        if (--o->timer != 0) break;
        o->timer = 30;
        o->d84 = 0;
        o->step++;
        break;
    case 6:
        o->d30 += ((X(o)->wc8 << 16) - o->d30) >> 2;
        o->d34 += ((X(o)->wca << 16) - o->d34) >> 2;
        o->d84 += 0x40;
        if (o->d84 < 0x400) break;
        if (X(o)->wcc < 0x80) {
            o->b04++;
            break;
        }
        off = X(o)->wc2 * 10;
        c0 = D_800B0BBC[off];
        c1 = D_800B0BBD[off];
        c2 = D_800B0BBE[off];
        c3 = D_800B0BBF[off];
        o->d84 = 0xc00;
        o->step++;
        o->active = 1;
        x = c0 << 2;
        y = c1 << 8;
        o->box1 = x | y;
        x2 = x + c2;
        y2 = y + (c3 << 8);
        o->box3 = y | x2;
        o->box0 = x | y2;
        o->box2 = x2 | y2;
        break;
    case 7:
        o->d84 += 0x40;
        if (o->d84 == 0x1000) {
            o->timer = (FUN_8001f9e0() & 0x1f) + 0x5a;
            o->step++;
        }
    case 8:
        if (--o->timer != 0) break;
        o->wb6 = (FUN_8001f9e0() & 0x7f) - 0x40;
        o->wba = (FUN_8001f9e0() & 0x7f) - 0x40;
        o->wb8 = (FUN_8001f9e0() & 0x7f) - 0x40;
        o->velY = FUN_8001f9e0() & 0x13;
        o->step++;
        break;
    case 9:
        o->d84 = (o->d84 + (unsigned short)o->wb6) & 0xfff;
        o->d88 = (o->d88 + (unsigned short)o->wba) & 0xfff;
        o->d8c = (o->d8c + (unsigned short)o->wb8) & 0xfff;
        o->velY += 2;
        o->d34 += (o->velY << 16) >> 4;
        if (o->d34 > 0x1100000) o->b04++;
        break;
    }
    o->a.p.whole = (o->d30 >> 16) + 0xa0;
    o->y.p.whole = (o->d34 >> 16) - 0x78;
    o->b.p.whole = (o->d38 >> 16);
}
