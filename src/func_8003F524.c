// FUNC 8003f524 116 MAIN0
// MATCHING 8003f524 116
#include "TOBJ.H"
int func_8003F524(TObj *o)
{
    unsigned short v = *(unsigned short *)0x1f800282;
    int k = (v >> 5) & 0xf;
    if ((v & 0x3000) == 0) {
        switch (k) {
        case 14: ((unsigned char *)o)[0xa1] = 2; break;
        case 13: ((unsigned char *)o)[0xa1] = 3; break;
        case 15: ((unsigned char *)o)[0xa1] = 1; break;
        }
        return ((unsigned char *)o)[0xa1];
    }
    return 0;
}
