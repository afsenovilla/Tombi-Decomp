// FUNC 80116920 1016 X017
// MATCHING 80116920 1016
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    short a, b, c, d, e, f, g, h, i, j;
} E118;

extern E118 D_800A3FE0[][7];
typedef struct { char pad[0xc2]; unsigned short wc2, wc4, wc6; short wc8; } X118;
#define X(o) ((X118 *)(o))
extern int D_1F8002D8;
extern char *D_800A4468;
extern char *D_800B0BB0;
extern char D_80012100[];
extern char D_800122A0[];
extern void FUN_80026a10(int);
extern TObj *FUN_80018448(void);
extern unsigned short GetClut(int, int);

void func_80116920(int n, short x, short y, short z, short t)
{
    int i;
    TObj *o;
    int cnt;
    short px;
    short py;
    short zero;
    short w800;
    E118 *e;
    unsigned char c;
    int h;
    unsigned short *dp;
    int off;
    char *q;
    int w, m;

    FUN_80026a10(n);
    for (i = 7; i >= 0; i--) {
        if (D_800A3FE0[i][0].a == -1)
            break;
    }
    if (i < 0)
        return;
    o = FUN_80018448();
    if (o == 0)
        return;
    h = n / 100000;
    o->wb4 = h % 10;
    o->type = 0;
    o->active = 1;
    o->b0a = 4;
    o->d3c = D_1F8002D8;
    o->w1e = 0x15;
    o->b0f = 5;
    o->a.raw = x << 16;
    o->y.raw = y << 16;
    o->b.raw = z << 16;
    S16(o, 0x2c) = 0;
    S16(o, 0x20) = t;
    S16(o, 0xc6) = i;
    o->wb6 = n / 10000 - h * 10;
    o->wb8 = n / 1000 - n / 10000 * 10;
    o->wba = n / 100 - n / 1000 * 10;
    o->wbc = n / 10 - n / 100 * 10;
    S16(o, 0xbe) = n - n / 10 * 10;
    if (t == 1)
        X(o)->wc8 = 0;
    else
        X(o)->wc8 = 1;
    if (n < 1000) {
        o->subtype = 0;
    } else {
        if (n < 10000) o->subtype = 1;
        else if (n < 20000) o->subtype = 2;
        else if (n > 49999) o->subtype = 4;
        else o->subtype = 3;
    }
    X(o)->wc2 = GetClut(0x210, 0x1e1);
    X(o)->wc4 = GetClut(0x210, o->subtype + 0x1e1);
    cnt = 0;
    px = 0;
    zero = 0;
    w800 = 800;
    dp = (unsigned short *)o;
    off = 0;
    py = 1;
    i = 0;
loop:
        w = X(o)->wc6;
        m = (w << 3) + w;
        m = (m << 2) - w;
        m = m << 2;
        q = (char *)D_800A3FE0 + off;
        e = (E118 *)(m + (int)q);
        e->a = 0;
        if (dp[0x5a] != 0 || cnt != 0) {
            e->b = py;
            e->a = 1;
            e->c = px * 100;
            e->d = zero;
            e->e = (px + 8) * 100;
            e->f = w800;
            e->g = 0;
            e->h = 0;
            e->i = 0;
            e->j = 0;
            px += 8;
            py += 4;
            cnt++;
        }
            dp++;
        i++;
        off += 20;
    if (i < 6) goto loop;
    D_800A4468 = D_80012100;
    D_800B0BB0 = D_800122A0;
    e = &D_800A3FE0[X(o)->wc6][6];
    e->a = 1;
    e->b = (cnt << 2) | 1;
    e->c = cnt * 400;
    e->d = -3200;
    e->e = cnt * 400;
    e->f = -3200;
}
