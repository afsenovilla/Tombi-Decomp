// FUNC 8003ab9c 36 MAIN0
// MATCHING 8003ab9c 36
#include "TOBJ.H"
extern char *D_8009F0F0;
extern unsigned char D_800A60D6;
void func_8003AB9C(void)
{
    char *p = D_8009F0F0;
    *(int *)(p + 0x1190) = D_800A60D6;
    *(short *)(p + 0x8a) += 1;
}
