// FUNC 80115ea8 228 X008
// MATCHING 80115ea8 228
#include "TOBJ.H"
typedef void (*Fn)(TObj *);

extern unsigned char D_8009C93A;
extern Fn D_80079AB4[];
extern short D_1F800172;
extern void FUN_8002a3d4(TObj *);
extern int FUN_800270a0(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void func_80115EA8(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        FUN_8002a3d4(o);
        break;
    case 1:
        D_80079AB4[o->subtype](o);
        if (D_1F800172 < -0x59) FUN_800270a0(o, 1, 0);
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
