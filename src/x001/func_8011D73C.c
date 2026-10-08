// FUNC 8011d73c 844 X001
// MATCHING 8011d73c 844
#include "TOBJ.H"

extern unsigned char D_8009CDBA;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC;
extern unsigned char D_8009C93F, D_8009C975, D_8009CE40, D_8009D093, D_8009D092;
extern unsigned char D_1F8001CC, D_1F8001CD, D_1F8003D0;
extern short D_1F8001C6, D_800A60EA;
extern char D_8001D6A4[];
extern int ObjCullRegister(TObj *);
extern void FUN_8001f110(int);
extern void FUN_8001f4bc(void);
extern void FUN_8001eb64(void);
extern void FUN_800171b8(int, char *);
extern void FUN_8005a9a4(int, int);
extern void ObjFreeDup(TObj *);

void func_8011D73C(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009CDBA < 2) {
            o->b04 = 2;
            break;
        }
        o->a.raw = 0x10520000;
        o->y.raw = 0xf9130000;
        o->b.raw = 0x05400000;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x18;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->active = 1;
        o->b69 = 0;
        o->b04++;
        if (o->subtype == 1) o->step = 1;
        else o->step = 0;
        o->state = 0;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (D_8009D2B0 == 1 && o->b69 && (D_1F8001FC & 0x10)) {
                o->step++;
                o->state = 0;
                o->b69 = 0;
            }
            break;
        case 1:
            switch (o->state) {
            case 0:
                FUN_8001f110(1);
                D_8009C93F = 1;
                D_8009C975 = 3;
                o->state++;
            case 1:
                if (D_8009C975 != 1) break;
                FUN_8001f4bc();
                D_1F8001CC = 1;
                if (D_8009CE40) {
                    D_1F8001CD = 8;
                    if (D_8009D093) {
                        D_1F8003D0 = 1;
                    } else {
                        D_1F8003D0 = 0;
                        D_8009D093 = 1;
                    }
                } else {
                    D_1F8001CD = 5;
                    if (!D_8009D092) {
                        D_1F8003D0 = 0;
                        D_8009D092 = 1;
                    } else {
                        D_1F8003D0 = 1;
                    }
                }
                D_1F8001C6 = 2;
                FUN_800171b8(1, D_8001D6A4);
                o->state = 0;
                o->step++;
                break;
            }
            break;
        case 2:
            switch (o->state) {
            case 0:
                D_8009C975 = 4;
                FUN_8001eb64();
                o->state++;
            case 1:
                if (D_8009C975) break;
                D_800A60EA = 0;
                D_8009C93F = 0;
                FUN_8005a9a4(0x16, 0);
                o->step = 0;
                break;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
