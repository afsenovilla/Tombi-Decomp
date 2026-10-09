// FUNC 8012e728 416 X010
// MATCHING 8012e728 416
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern void func_8012E550(TObj *);
extern void FUN_8005a9a4(int, int);
extern void PoolFree_1F800210(TObj *);

void func_8012E728(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012E550(o);
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            break;
        case 1:
            switch (o->state) {
            case 0:
            case 1:
                if (--o->w08 <= 0) o->state = 15;
                break;
            case 2:
                D_800A603C = 5;
                D_800A603D = 100;
                D_800A603E = 0;
                D_8009C93F = 1;
                D_8009C942 = 1;
                D_8009C93E = 1;
                FUN_8005a9a4(0xb7, 0);
                o->w08 = 200;
                o->state = 1;
                break;
            case 15:
                D_800A603C = 1;
                D_800A603D = 0;
                D_800A603E = 0;
                D_8009C93F = 0;
                D_8009C942 = 0;
                D_8009C93E = 0;
                o->step = 0;
                o->state = 0;
                break;
            }
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
