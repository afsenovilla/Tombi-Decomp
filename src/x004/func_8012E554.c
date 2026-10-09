// FUNC 8012e554 368 X004
// MATCHING 8012e554 368
#include "TOBJ.H"

extern unsigned char D_800A60DA;
extern unsigned short *D_800A6078;
extern short D_800A604E;
extern unsigned char D_8009CDDB;
extern unsigned char D_8009D0D9;
extern unsigned char D_8009CDD7;
extern unsigned char D_8009D0DA;
void func_8012DFBC(TObj *o);
void func_8012E204(TObj *o);

void func_8012E554(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_800A60DA != 1)
            break;
        if ((unsigned short)(D_800A6078[1] - 0x370) < 0x28 && D_800A604E == -0x1d6) {
            if (D_8009CDDB == 0) {
                o->step = 1;
                o->state = 0;
            } else if (D_8009D0D9 != 0) {
                o->step = 1;
                o->state = 2;
            }
        }
        if ((unsigned short)(D_800A6078[1] - 0x3c8) < 0x28 && D_800A604E == -0x2e4) {
            if (D_8009CDD7 == 0) {
                o->step = 2;
                o->state = 0;
            } else if (D_8009D0DA != 0) {
                o->step = 2;
                o->state = 2;
            }
        }
        break;
    case 1:
        func_8012DFBC(o);
        break;
    case 2:
        func_8012E204(o);
        break;
    }
}
