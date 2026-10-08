// FUNC 80135b48 328 X001
// MATCHING 80135b48 328
#include "TOBJ.H"

extern unsigned short D_8009D2C0;
extern unsigned char D_8009CDB5;
extern unsigned char D_8009CEC1, D_8009CEC2, D_8009CEC3, D_8009CEC4, D_8009CEC5, D_8009CEC6, D_8009CEC7;
extern void *func_8004D620(int, int);
extern void func_80135A14(TObj *);

void func_80135B48(TObj *o)
{
    if (o->b0c != 0) {
        unsigned short *f = &D_8009D2C0;
        *f |= 1 << (o->b0c - 1);
    }
    {
        unsigned char *c = &D_8009CDB5;
        *c = *c + 1;
        if (*c >= 9) *c = 8;
        else func_8004D620(*c - 1, 2);
    }
    switch (o->b0c) {
    case 1:
        D_8009CEC2 = 1;
        break;
    case 2:
        D_8009CEC1 = 1;
        break;
    case 3:
        D_8009CEC4 = 1;
        break;
    case 4:
        D_8009CEC3 = 1;
        break;
    case 5:
        D_8009CEC5 = 1;
        break;
    case 6:
        D_8009CEC6 = 1;
        break;
    case 7:
        D_8009CEC7 = 1;
        break;
    }
    func_80135A14(o);
}
