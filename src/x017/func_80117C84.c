// FUNC 80117c84 536 X017
// MATCHING 80117c84 536
#include "TOBJ.H"
extern void func_80117224(TObj *);
extern void func_8011750C(TObj *);
extern void func_80117864(TObj *);
extern void func_801179EC(TObj *);
extern void FUN_8005a9a4(int, int);
extern unsigned char D_8009C940;
extern unsigned char D_8009C941;
extern unsigned char D_8009CE0A;
extern unsigned char D_8009CFD6;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93F;
extern unsigned char D_8009D2B0;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;

void func_80117C84(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C940 != 0) {
            if (D_8009C941 == 0x3c && D_8009CE0A == 1) {
                o->step = 3;
                o->state = 0;
            }
            D_8009C940 = 0;
        } else {
            if (o->b68 != 0 && (unsigned short)o->wba == 1) {
                o->step = 5;
                o->state = 0;
            }
            if (D_8009CE0A != 0xff) {
                if (D_8009CFD6 == 1) o->step = 4;
                else o->step = 1;
                o->state = 0;
            }
        }
        break;
    case 1:
        func_80117224(o);
        break;
    case 2:
        func_8011750C(o);
        break;
    case 3:
        func_80117864(o);
        break;
    case 4:
        switch (o->state) {
        case 0:
            if (D_8009C93A != 0) {
                D_8009C942 = 1;
                D_8009C93F = 1;
                D_8009D2B0 = 2;
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                FUN_8005a9a4(0x66, 0);
                o->timer = 300;
                o->state++;
            }
            break;
        case 1:
            if (--o->timer <= 0) {
                D_8009CFD6 = 1;
                D_800A603C = 1;
                D_800A603D = 0x20;
                D_800A603E = 0;
                o->state++;
            }
            break;
        }
        break;
    case 5:
        func_801179EC(o);
        break;
    }
}
