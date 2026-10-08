// FUNC 80047a3c 180 MAIN0
// MATCHING 80047a3c 180
#include "TOBJ.H"
extern unsigned char D_8009C938;
extern signed char D_8009D2B0;
extern short D_1f80019e;
extern unsigned short D_1f800248;
extern unsigned char **D_1f800228;
extern void FUN_8004432c(TObj *o, unsigned char *p);
void func_80047A3C(TObj *o)
{
    unsigned char **pp;
    unsigned char *p;
    if (D_8009C938 != 0) return;
    if (D_8009D2B0 == 3) return;
    pp = D_1f800228;
    D_1f80019e = D_1f800248;
    while (D_1f80019e != 0) {
        p = *pp++;
        D_1f80019e -= 1;
        if (*p & 3) {
            FUN_8004432c(o, p);
        }
    }
}
