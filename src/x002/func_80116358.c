// FUNC 80116358 644 X002
// MATCHING 80116358 644
#include "TOBJ.H"
extern unsigned char D_8009C93A;
extern unsigned short D_8009C962;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_800A604A, D_800A604E;
extern void (*D_80079AD0[])(TObj *);
extern void func_80027810(TObj *);
extern void FUN_800270a0(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void func_80116358(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) {
            o->step++;
            o->subtype = 0;
        }
        ((short *)o->d38)[1] = 0;
        func_80027810(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (D_8009C962) {
        case 1:
            if (D_1F800172 == 0) {
                if (D_800A604A < 30 && D_800A604E >= -59)
                    FUN_800270a0(o, 1, 0);
            } else {
                if (D_1F80016A < 30 && D_1F80016E >= -77)
                    FUN_800270a0(o, 1, 1);
            }
            break;
        case 2:
            if (D_1F80016A < 30 && D_1F80016E >= -59)
                FUN_800270a0(o, 1, 0);
            break;
        case 4:
            if (D_1F80016A < 28 && D_1F80016E >= -87)
                FUN_800270a0(o, 1, 0);
            break;
        case 5:
            if (D_1F80016A >= 0x128 && D_1F80016E >= -59)
                FUN_800270a0(o, 1, 0);
            else if (D_1F800172 >= 0x2bd)
                FUN_800270a0(o, 1, 2);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
