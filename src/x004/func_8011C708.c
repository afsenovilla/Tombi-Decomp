// FUNC 8011c708 392 X004
// MATCHING 8011c708 392
#include "TOBJ.H"
extern unsigned char D_8009CDA2;
extern unsigned short D_8009C962;
extern short D_1F800172;
extern short D_1F80016A;
extern short D_1F800176;
extern short *D_800A6078;
extern void FUN_80018cf0(TObj *);
extern void FUN_80018838(TObj *);

void func_8011C708(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        switch (o->subtype) {
        case 0:
            break;
        case 1:
            o->d84 = 0;
            o->d88 = 0xc00;
            o->d8c = 0;
            break;
        case 2:
            o->d84 = 0;
            o->d88 = 0x800;
            o->d8c = 0;
            break;
        }
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            if (D_8009CDA2 == 0 && D_8009C962 == 1) break;
            if (D_1F800172 == 0x32a) {
                if (D_1F80016A < 0x92) D_800A6078[1] = 0x92;
            }
            o->visible = 1;
            FUN_80018cf0(o);
            break;
        case 1:
            if (D_8009C962 == 2 && D_1F800176 < 0x456) break;
            o->visible = 1;
            FUN_80018cf0(o);
            break;
        case 2:
            o->visible = 1;
            FUN_80018cf0(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
