// FUNC 8012e328 352 X010
// MATCHING 8012e328 352
#include "TOBJ.H"

extern unsigned char D_8009CE5B;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern void FUN_8005a8a8(int, int, int);

void func_8012E328(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE5B == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            D_800A603C[0] = 5;
            D_800A603D[0] = 100;
            D_800A603E[0] = 0;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_8009C93E[0] = 1;
            FUN_8005a8a8(0xb7, 0, 0);
            o->w08 = 200;
            o->state++;
            break;
        case 1:
            if (--o->w08 > 0)
                break;
            o->state = 0xf;
            break;
        case 2:
            break;
        case 0xf:
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_8009C93E[0] = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    }
}
