// FUNC 80133938 232 X003
// MATCHING 80133938 232
#include "TOBJ.H"
extern unsigned char D_800A60DA;
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern unsigned char D_8009CE4E;
extern unsigned char D_8009D2C3;
extern void func_801337B4(TObj *);

void func_80133938(TObj *o)
{
    unsigned char c;

    switch (o->step) {
    case 0:
        c = D_800A60DA;
        if (c == 1 && (unsigned short)(D_800A6078->p.whole - 0xc8e) < 0x28 && D_800A604E == -0x7bb) {
            switch (D_8009CE4E) {
            case 0:
                o->step = 1;
                o->state = 0;
                break;
            case 1:
                if (D_8009D2C3 & 2) {
                    o->step = 1;
                    o->state = 2;
                }
                break;
            }
        }
        break;
    case 1:
        func_801337B4(o);
        break;
    }
}
