// FUNC 80118544 540 X014
/* score 12 (n55, was 14): `int x = ((int *)*k)[2]; o->da0 = *k + x;` (block-local volatile int *k) gives the game's
   lw v0/lw v1/lw a0,8(v0) and `addu v1,v1,a0` (sum into the second read's reg; the old `[2] + *k` form put x first).
   Left: the game stores da0 AFTER the three zero stores and keeps the sum addu right after the lbu b0c; here the
   sw da0 comes before the zero stores (source order). Writing `s = *k + x;` early and `o->da0 = s;` after the zero
   stores gives the game's store order but local-alloc then puts the sum in a0, x in a2, o->d in v1 and t in a1
   (game v1/a0/a0/v0) and sched2 hoists lw o->d (score 22). Exhaustive orders of {x/sum, da0, ba4, velV, t=a[],
   zero stores, o->d->p.whole += t} with short/int t and a `short *p`/`Fix16 *d` temp (3780 variants) never beat 12;
   `s = *k; s += x;` (multi-set, n52) does not move the addu either. Real start of csv piece 80118568 (36+504 B). */
#include "TOBJ.H"

extern int D_1F800334;
extern int Rand(void);
extern int ObjCullRegister(TObj *);

void func_80118544(TObj *o)
{
    short a[4] = { -90, 0, 90, 180 };
    short b[8] = { 0, 45, 90, 135, 180, 225, 270, 315 };

    switch (o->step) {
    case 0: {
        volatile int *k = &D_1F800334; int x; short t;
        o->step++;
        x = ((int *)*k)[2];
        o->da0 = *k + x;
        o->ba4 = 1;
        o->velV = 0x600;
        t = a[o->b0c & 3];
        o->d84 = 0; o->d88 = 0; o->d8c = 0;
        o->d->p.whole += t;
        o->h->p.whole += b[Rand() & 7];
        break;
    }
    case 1:
        o->y.raw += o->velV << 8;
        if (ObjCullRegister(o)) o->step++;
        break;
    case 2:
        o->y.raw += o->velV << 8;
        switch (o->b0c & 1) {
        case 0:
            o->d84 += 0x40;
            o->d8c += 0x20;
            break;
        case 1:
            o->d84 += 0x20;
            o->d8c += 0x40;
            break;
        }
        if (!ObjCullRegister(o)) o->b04 = 3;
        break;
    }
}
