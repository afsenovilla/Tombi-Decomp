// VERSION BETA_980203 beta980203 80033684 244
#if defined(VERSION_BETA_980203)
// Beta version: written from the disc. A static object: state 0 sets its box from the subtype (0 or 1), state 1 registers it in the 1F800224 list (subtype 0) or culls it, state 3 frees it.
#include "raw7.h"
extern void ObjListPush_1F800224();
extern void ObjCullRegister();
extern void ObjFreeDup();

void func_80033684_beta980203(char *o)
{
    unsigned char s = U8(o, 4);
    switch (s) {
    case 0:
        U8(o, 4) = s + 1;
        if (U8(o, 0xc) == 0) {
            S16(o, 0x6c) = 0x50;
            S16(o, 0x6e) = 0xa0;
            S16(o, 0x70) = 0x40;
            S16(o, 0x72) = 0x5c;
        } else if (U8(o, 0xc) == 1) {
            S16(o, 0x6c) = 0x50;
            S16(o, 0x6e) = 0xa0;
            S16(o, 0x70) = 0x18;
            S16(o, 0x72) = 0x24;
        }
        S32(o, 0xa0) = 0;
        break;
    case 1:
        if (U8(o, 0xc) == 0) {
            U8(o, 1) = 1;
            ObjListPush_1F800224(o);
        } else {
            ObjCullRegister(o);
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
