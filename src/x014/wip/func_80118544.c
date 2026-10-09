// FUNC 80118544 540 X014
/* score 14: real start of csv piece 80118568 (36+504 B). Left: case 0 scheduling: game keeps the da0 sum and the
   a[] / o->d loads above the zero stores (sw 84/88/8c, da0 after the lhu a[]); every order of the C statements
   gives the stores first. Temps before the stores fix the store placement but shift registers (20). */
#include "TOBJ.H"

extern int D_1F800334;
extern int Rand(void);
extern int ObjCullRegister(TObj *);

void func_80118544(TObj *o)
{
    short a[4] = { -90, 0, 90, 180 };
    short b[8] = { 0, 45, 90, 135, 180, 225, 270, 315 };

    switch (o->step) {
    case 0:
        o->step++;
        { volatile int *k = &D_1F800334; o->da0 = ((int *)*k)[2] + *k; }
        o->ba4 = 1;
        o->velV = 0x600;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d->p.whole += a[o->b0c & 3];
        o->h->p.whole += b[Rand() & 7];
        break;
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
