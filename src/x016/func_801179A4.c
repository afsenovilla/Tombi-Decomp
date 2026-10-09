// FUNC 801179a4 412 X016
// MATCHING 801179a4 412
#include "TOBJ.H"

extern unsigned char D_8009CE05;
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern unsigned char D_8009C940, D_8009C941, D_8009C93E;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC;
extern void func_801174B8(TObj *);
extern void func_8011767C(TObj *);
extern void FUN_80018980(TObj *);

void func_801179A4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009CE05 == 0xff) break;
            if ((unsigned short)(D_800A6078->p.whole - 0x82) >= 0x28) break;
            if (D_800A604E != -0x3d) break;
            if (D_8009C940 != 0 && D_8009C941 == 0x3e) {
                o->step = 2;
                o->state = 0;
                break;
            }
            if ((*(volatile unsigned short *)&D_8009D670 & 0x10) && (D_1F8001FC & 0x2000)) {
                D_8009C93E = 1;
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            func_801174B8(o);
            break;
        case 2:
            func_8011767C(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
