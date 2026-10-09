// FUNC 8012b454 392 X003
// MATCHING 8012b454 392
#include "TOBJ.H"

extern unsigned char D_8009C940;
extern unsigned char D_8009C941;
extern unsigned char D_8009D003;
extern void func_8012B2E4(TObj *);
extern void func_8012AB5C(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_80018790(TObj *);

static __inline__ int check(void)
{
    return D_8009C940 == 1 && D_8009C941 == 0x7d;
}

void func_8012B454(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012B2E4(o);
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b0c == 0x28) {
            switch (o->step) {
            case 0:
                if (o->b68) {
                    o->step++;
                } else if (check()) {
                    D_8009C940 = 0;
                    if (D_8009D003 == 0) D_8009D003 = 1;
                    o->step++;
                }
                break;
            case 1:
                func_8012AB5C(o);
                break;
            }
        }
        if (o->anim && o->active) FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
