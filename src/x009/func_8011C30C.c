// FUNC 8011c30c 392 X009
// MATCHING 8011c30c 392
#include "TOBJ.H"

extern unsigned char D_800A60DA;
extern unsigned int D_8009C96C;
extern unsigned char D_8009CDB4;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_8011C30C(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b69 == 1 && D_800A60DA == 1) o->step++;
        break;
    case 1:
        if (D_8009C96C <= 0x249ef) {
            if (D_8009CDB4 == 0) {
                FUN_8005a8a8(0x10, 0, 0);
                o->step++;
            } else {
                o->step--;
            }
        } else if (D_8009CDB4 != 0xff) {
            FUN_8005a9a4(0x10, 0);
            o->step++;
        } else {
            o->b04 = 2;
            o->step = 0;
        }
        break;
    case 2:
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        o->timer = 0x168;
        o->step++;
        break;
    case 3:
        if (--o->timer == -1) o->step++;
        break;
    case 4:
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        o->step = 1;
        break;
    }
}
