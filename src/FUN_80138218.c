// FUNC 80138218 516 X000
// MATCHING 80138218 516
#include "TOBJ.H"
typedef struct { unsigned char active, b1, type; } P;
extern unsigned char D_8009d005[];
extern unsigned char DAT_800a60d6;
extern short DAT_800a60ea;
extern unsigned char DAT_8009c93f, DAT_8009c93e;
extern P *FUN_80018448(void);
extern void FUN_80018980(void);
extern void FUN_8011b4b0(TObj *);

void FUN_80138218(TObj *o)
{
    P *p;
    unsigned char two;
    switch (o->subtype) {
    case 0:
        switch (D_8009d005[0]) {
        case 0:
            switch (o->step) {
            case 0:
                if (DAT_800a60d6 == 3) {
                    o->step++;
                    o->w08 = 0xf;
                }
                break;
            case 1:
                if (--o->w08 == 0) {
                    o->step = 0;
                    if (D_8009d005[0] == 0) D_8009d005[0] = 1;
                    DAT_8009c93f = 1;
                    DAT_8009c93e = 1;
                    p = FUN_80018448();
                    if (p != 0) {
                        p->active = 2;
                        p->type = 0x59;
                    }
                }
                break;
            }
            break;
        case 1:
            switch (o->step) {
            case 0:
                if (DAT_800a60d6 == 3 && DAT_800a60ea == 4) {
                    o->w08 = 0xf;
                    o->step++;
                }
                break;
            case 1:
                if (--o->w08 == 0) {
                    two = 2;
                    o->step = 0;
                    D_8009d005[0] = two;
                    DAT_8009c93f = 1;
                    DAT_8009c93e = 1;
                    p = FUN_80018448();
                    if (p != 0) {
                        p->active = two;
                        p->type = 0x59;
                    }
                }
                break;
            }
            break;
        case 2:
            FUN_80018980();
            break;
        }
        break;
    case 1:
        FUN_8011b4b0(o);
        break;
    }
}
