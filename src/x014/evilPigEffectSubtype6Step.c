// FUNC 80118544 540 X014
// MATCHING 80118544 540
#include "spad.h"
#include "TOBJ.H"

extern int Rand(void);
extern int ObjCullRegister(TObj *);

void evilPigEffectSubtype6Step(TObj *o)
{
    const short a[4] = { -90, 0, 90, 180 };
    const short b[8] = { 0, 45, 90, 135, 180, 225, 270, 315 };

    switch (o->step) {
    case 0: {
        volatile int *k = &SPAD.i334;
        int x;
        o->step++;
        x = ((int *)*k)[2];
        o->da0 = *k + x;
        o->ba4 = 1;
        o->velV = 0x600;
        o->d84 = 0; o->d88 = 0; o->d8c = 0;
        o->d->p.whole += a[o->b0c & 3];
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
