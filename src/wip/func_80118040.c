// FUNC 80118040 1016 X000
/* score 78 (b35: o->d3c = D_1F8002D8 moved right after o->type = 0, found by line hill-climb; raw-offset stores did not help; t as ushort/int/char worse): whole range 80118040+801180ac+8011832c (splat split it). Remaining: prologue save order of s2,
   scheduling of the early field stores vs the division mults, subtype chain value in v1 instead of v0,
   and loop hoisting (game hoists 800 and keeps a zero var in $11 but re-materializes the table base
   and the constant 1 every iteration). Tried: store placement, statement perms, e/offset/digit-pointer forms. */
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
extern void FUN_80026a10(void);
extern TObj *FUN_80018448(void);
extern unsigned short GetClut(int, int);

void func_80118040(int n, int x, int y, int z, short t)
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

    FUN_80026a10();
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
    o->type = 0;
    o->active = 1;
    o->d3c = D_1F8002D8;
    o->b0a = 4;
    o->w1e = 0x15;
    o->b0f = 5;
    o->a.raw = x << 16;
    o->y.raw = y << 16;
    o->b.raw = z << 16;
    o->animTimer = 0;
    o->timer = t;
    X(o)->wc6 = i;
    o->wb6 = n / 10000 - h * 10;
    o->wb8 = n / 1000 - n / 10000 * 10;
    o->wba = n / 100 - n / 1000 * 10;
    o->wbc = n / 10 - n / 100 * 10;
    S16(o, 0xbe) = n - n / 10 * 10;
    o->wb4 = h % 10;
    if (t == 1)
        X(o)->wc8 = 0;
    else
        X(o)->wc8 = 1;
    if (n < 1000) {
        o->subtype = 0;
    } else {
        c = 1;
        if (n >= 10000) {
            c = 2;
            if (n >= 20000) {
                c = 4;
                if (n < 50000)
                    c = 3;
            }
        }
        o->subtype = c;
    }
    X(o)->wc2 = GetClut(0x210, 0x1e1);
    X(o)->wc4 = GetClut(0x210, o->subtype + 0x1e1);
    cnt = 0;
    px = 0;
    py = 1;
    zero = 0;
    w800 = 800;
    for (i = 0; i < 6; i++) {
        e = (E118 *)(X(o)->wc6 * 140 + (i * 20 + (int)D_800A3FE0));
        e->a = 0;
        if (((unsigned short *)&o->wb4)[i] != 0 || cnt != 0) {
            e->b = py;
            py += 4;
            cnt++;
            e->a = 1;
            e->c = px * 100;
            e->d = zero;
            e->e = (px + 8) * 100;
            e->f = 800;
            e->g = 0;
            e->h = 0;
            e->i = 0;
            e->j = 0;
            px += 8;
        }
    }
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
