// FUNC 801165bc 424 X006
// MATCHING 801165bc 424
#include "TOBJ.H"

typedef void (*Fn)(TObj *);
extern Fn D_80079AD0[];
extern unsigned char D_8009C93A;
extern unsigned short D_8009C962, D_800A604A;
extern short D_800A6052;
extern short D_1F80016A, D_1F80016E;
extern void FUN_80027810(TObj *);
extern int FUN_800270a0(TObj *, int, unsigned char);
extern void FUN_8002a4d0(TObj *);

void func_801165BC(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A) {
            o->step++;
            o->subtype = 0;
        }
        FUN_80027810(o);
        ((short *)o->d38)[1] = 0;
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (D_8009C962) {
        case 1:
            if ((unsigned short)(D_800A604A - 0x2e7) < 0x80 && D_800A6052 >= 0x5b) {
                FUN_800270a0(o, 1, 0);
            } else if (D_1F80016A >= 0x3ad && D_1F80016E >= -0x3b) {
                FUN_800270a0(o, 1, 1);
            }
            break;
        case 2:
            if (D_1F80016A < 0x1e && D_1F80016E >= -0x3b)
                FUN_800270a0(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
