// FUNC 80125c50 348 X010
// MATCHING 80125c50 348
#include "TOBJ.H"
extern unsigned char D_8009C940;
extern unsigned char D_8009C941;
extern void func_80125AE0(TObj *);
extern void func_801253AC(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_80018790(TObj *);

static __inline__ int chk(void)
{
    return D_8009C940 == 1 && D_8009C941 == 0x9f;
}

void func_80125C50(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80125AE0(o);
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b0c == 0x28) {
            switch (o->step) {
            case 0:
                if (o->b68 || chk()) o->step++;
                break;
            case 1:
                func_801253AC(o);
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
