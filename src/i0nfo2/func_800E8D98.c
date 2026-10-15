// FUNC 800e8d98 316 I0NFO2
// MATCHING 800e8d98 316
#include "TOBJ.H"
extern void func_800270A0(TObj *o, int a, int b);
extern void func_80027810(TObj *o);
extern void func_8002A4D0(TObj *o);
extern void (*D_80079AD0[])(TObj *o);
extern unsigned char D_8009C93A;
extern short D_800A604A;
extern unsigned short D_800A604E;

void func_800E8D98(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        break;
    case 1:
        if (D_800A604A > 0x22B) {
            if ((unsigned)(D_800A604E - 0x169) < 0x64) func_800270A0(o, 1, 0);
        } else if ((short)D_800A604E < 0x4F) {
            if ((unsigned short)(D_800A604A - 0xC7) < 0x41) func_800270A0(o, 1, 1);
        }
        D_80079AD0[o->subtype](o);
        break;
    case 2:
        func_8002A4D0(o);
        break;
    }
}
