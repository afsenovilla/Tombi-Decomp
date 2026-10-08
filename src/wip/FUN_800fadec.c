// FUNC 800fadec 680 X000
// w1: score 24; st lives in v1 (game v0, li 2 in bnez delay slot), w/c regs swapped later. Tried st types, if/else forms, o->step per branch (57), reusing f/n/d: no gain.
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pc0[7];
    unsigned char bc7;
    unsigned char bc8;
} TX;
typedef struct {
    char p0[8];
    unsigned char b8;
    char p9[0x17];
    short w20;
    short w22;
} G330;
extern unsigned short DAT_1f8001fc, DAT_1f8003c6, DAT_1f8003c8;
extern int DAT_8009c934;
extern G330 *DAT_8009c330;
extern volatile unsigned short DAT_8009d670[];
extern unsigned char DAT_801152e8[];
extern short FUN_800411cc(TObj *o, int x, int y);
extern short FUN_8003fd78(TObj *o, int a, int b);
extern void FUN_8001e5f4(int a, int b);

#define o (&x->t)

void FUN_800fadec(TX *x)
{
    G330 *g;
    short f;
    int d;
    short n;
    int st;
    int w;
    unsigned char c;
    if (DAT_1f8001fc & (DAT_1f8003c6 | DAT_1f8003c8)) {
        DAT_8009c934 = 0;
        x->bc7 = 1;
        o->ba4 = 0;
        *(unsigned char *)&o->wac = 0;
        o->b69 = 0;
        o->b9c = 0;
        o->b9d = 0;
        x->bc8 = 0;
        o->ba7 = 0;
        g = DAT_8009c330;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        g->b8 = 0;
        g = DAT_8009c330;
        g->w20 = 0;
        g->w22 = 0;
        f = 0;
        if (o->wb4 < 0x29)
            f = (DAT_8009d670[0] >> 4) & 1;
        if (f) {
            o->h->p.whole = o->d30;
            d = o->d34;
            o->y.p.whole = d + 0x14;
            n = FUN_800411cc(o, o->h->p.whole, (short)(d - 0x2c)) != 0;
            if (FUN_800411cc(o, o->h->p.whole, (short)(o->y.p.whole - 0x50)))
                n++;
            if (FUN_800411cc(o, o->h->p.whole, (short)(o->y.p.whole - 0x60)))
                n++;
            st = 2;
            if (n == 0) {
                *(signed char *)&o->b0f = -20;
                st = 0x49;
                *(unsigned char *)&o->waa = 1;
            }
        } else {
            st = 0x37;
        }
        o->step = st;
        o->state = 0;
    } else {
        if (o->b69 == 1) {
            g = DAT_8009c330;
            g->w20 = 0;
            g->w22 = 0;
            g->b8 = 0;
            DAT_8009c934 = 0;
            FUN_8001e5f4(0x1c, 0x7f);
            w = o->wb0;
            x->bc7 = 1;
            o->b9c = 0;
            *(unsigned char *)&o->wac = 0;
            o->b9d = 0;
            x->bc8 = 0;
            c = DAT_801152e8[w];
            o->step = 0;
        } else {
            if (FUN_8003fd78(o, 0, 0) == 0)
                return;
            g = DAT_8009c330;
            g->w20 = 0;
            g->w22 = 0;
            g->b8 = 0;
            DAT_8009c934 = 0;
            FUN_8001e5f4(0x1c, 0x7f);
            w = o->wb0;
            x->bc7 = 1;
            o->b9c = 0;
            *(unsigned char *)&o->wac = 0;
            o->b9d = 0;
            x->bc8 = 0;
            c = DAT_801152e8[w];
            o->step = 1;
        }
        o->state = 0;
        o->d8c = c;
    }
}
