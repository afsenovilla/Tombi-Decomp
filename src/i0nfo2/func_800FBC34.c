// FUNC 800fbc34 312 I0NFO2
// MATCHING 800fbc34 312
#include "TOBJ.H"
extern unsigned char D_8009CFA8[];
extern signed char D_800A603C, D_800A603D, D_800A603E;

void func_800FBC34(TObj *o)
{
    switch (o->state) {
    case 0:
        D_800A603C = 4;
        D_800A603D = 0;
        D_800A603E = 0;
        o->w08 = 0x1E;
        o->state++;
        break;
    case 1:
        if (--o->w08 <= 0) {
            D_8009CFA8[0]++;
            o->w08 = 0x1E;
            o->state++;
        }
        break;
    case 2:
        if (--o->w08 <= 0) {
            D_8009CFA8[0]++;
            o->state++;
        }
        break;
    case 3:
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        D_8009CFA8[0]++;
        o->step = 0;
        o->state = 0;
        break;
    }
}
