// FUNC 8012bcb8 164 X003
// MATCHING 8012bcb8 164
#include "TOBJ.H"

extern unsigned char D_8009CE5D;
extern void func_8012B66C(TObj *);

void func_8012BCB8(TObj *o)
{
    if (o->b0c != 0x28) return;
    switch (o->step) {
    case 0:
        if (o->b68) o->step++;
        if (D_8009CE5D != 0xff) break;
        if (o->step != 0) break;
        o->d88 = 1;
        o->step = 1;
        o->state = 0x46;
        break;
    case 1:
        func_8012B66C(o);
        break;
    }
}
