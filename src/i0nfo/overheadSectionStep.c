// FUNC 800e8214 308 I0NFO
// MATCHING 800e8214 308
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char D_8009C93A;
extern short D_800A604A;
extern unsigned short D_800A604E;

void overheadSectionStep(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        if (D_800A604A < -0x70) {
            if ((unsigned)(D_800A604E - 0x138) < 0x21) func_800270A0(o, 1, 0);
        } else if (D_800A604A > 0x118) {
            if ((unsigned)(D_800A604E - 0x100) < 0x77) func_800270A0(o, 1, 1);
        }
        D_80079AD0[o->subtype](o);
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}
