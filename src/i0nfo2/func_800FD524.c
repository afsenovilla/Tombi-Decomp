// FUNC 800fd524 356 I0NFO2
// MATCHING 800fd524 356
#include "TOBJ.H"
extern signed char D_8009C93F, D_8009C942, D_800A6039, D_800A603C, D_800A603D, D_800A603E;
extern short D_800A604A[];
extern unsigned short D_800A604E[];
extern short D_800A6066;

void func_800FD524(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_800A604A[0] = 0x120;
        D_800A604E[0] = 0x110;
        D_800A6066 = 6;
        D_800A6039 = 0;
        D_800A603C = 4;
        D_800A603D = 0;
        D_800A603E = 0;
        o->w08 = 0x91;
        o->state++;
        break;
    case 1:
        if (--o->w08 >= 0) {
            if ((o->w08 & 1) && D_800A604A[0] < 0x12E) D_800A604A[0]++;
            D_800A604E[0]++;
            if (o->w08 >= 0xB) D_800A6039 = 0;
        } else {
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_800A6039 = 1;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
