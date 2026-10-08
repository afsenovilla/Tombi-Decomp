// FUNC 80121a5c 132 X000
// MATCHING 80121a5c 132
#include "TOBJ.H"
extern TObj *FUN_80018448(void);

TObj *FUN_80121a5c(unsigned char a, int x, int y, int z)
{
    TObj *p = FUN_80018448();

    if (p) {
        p->b0c = a;
        p->active = 1;
        p->type = 0x44;
        p->a.raw = x << 16;
        p->y.raw = y << 16;
        p->b.raw = z << 16;
    }
    return p;
}
