// FUNC 800ee5a4 248 I0NFO3
// MATCHING 800ee5a4 248
#include "TOBJ.H"
extern void func_8005A9A4(int a, int b);
extern unsigned char D_8009C93A;
extern signed char D_8009C93F, D_8009C942, D_800A603C, D_800A603D, D_800A603E, D_800A60E4;

void tunnelStartEvent4FStep(TObj *o)
{
    switch (o->state) {
    case 0:
        if (D_8009C93A != 0) {
            D_800A603D = 2;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A603E = 0;
            o->w08 = 0xC8;
            func_8005A9A4(0x4F, 0);
            o->state++;
        }
        break;
    case 1:
        if (--o->w08 <= 0) {
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
