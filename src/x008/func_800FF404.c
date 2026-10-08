// FUNC 800ff404 524 X008
// MATCHING 800ff404 524
#include "TOBJ.H"

extern unsigned char D_8009C990;
extern int D_8009C960;

short func_800FF404(TObj *o, short r)
{
    switch (D_8009C990 & 3) {
    case 0:
        r = 1;
        if (D_8009C960 == 0x20003) r = 2;
        if (D_8009C960 == 10) r = (o->h->p.whole < 0xc80) * 2;
        if (D_8009C960 == 0x3000a) r = 2;
        break;
    case 1:
        r = 2;
        if (D_8009C960 == 0x20000) r = 1;
        if (D_8009C960 == 0x30001) r = 1;
        if (D_8009C960 == 0x20003) r = 0;
        if (D_8009C960 == 10 && o->h->p.whole > 0xc80) r = 0;
        if (D_8009C960 == 0x3000a) r = 2;
        break;
    case 2:
        r = D_8009C960 == 0x20000;
        if (D_8009C960 == 0x30001) r = 1;
        if (D_8009C960 == 0x20003) r = 6;
        if (D_8009C960 == 10 && o->h->p.whole < 0xc80) r = 2;
        if (D_8009C960 == 0x3000a) r = 1;
        break;
    case 3:
        r = 2;
        if (D_8009C960 == 0x20003) r = 4;
        if (D_8009C960 == 10 && o->h->p.whole < 0xc80) r = 4;
        if (D_8009C960 == 0x3000a) r = 4;
        break;
    }
    return r;
}
