// FUNC 80027810 332 MAIN0
// MATCHING 80027810 332
#include "TOBJ.H"
typedef struct {
    char p0[0x2c];
    short s2c, s2e, s30, s32;
    Fix16 *f34;
} S;
typedef struct { char p[0x14]; int d14; } G;
extern unsigned char D_8009C939;
extern Fix16 *D_800A6078[];
extern G D_800A6038;
extern signed char D_8009D2B0[];
extern Fix16 D_1F8000F0;

void func_80027810(S *o)
{
    Fix16 *f;
    G *g;
    Fix16 *c;
    int v;

    if (D_8009C939 != 0) return;
    o->f34->raw = D_800A6078[0]->raw;
    f = o->f34;
    g = &D_800A6038;
    if (o->s2e < f->p.whole) {
        f->raw = o->s2e << 16;
        if (D_8009D2B0[0] != 3) {
            if (o->s2e + 150 < D_800A6078[0]->p.whole) D_800A6078[0]->p.whole = o->s2e + 150;
        }
    } else if (f->p.whole < o->s2c) {
        f->raw = o->s2c << 16;
        if (D_8009D2B0[0] != 3) {
            if (D_800A6078[0]->p.whole < o->s2c - 150) D_800A6078[0]->p.whole = o->s2c - 150;
        }
    }
    if (D_8009D2B0[0] < 3) {
        c = &D_1F8000F0;
        c->raw = g->d14;
        if (D_1F8000F0.p.whole > o->s32) {
            c->raw = o->s32 << 16;
        } else if (D_1F8000F0.p.whole < o->s30) {
            c->raw = o->s30 << 16;
        }
    }
}
