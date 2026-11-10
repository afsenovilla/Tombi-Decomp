// VERSION PCPX_96084 tomba 80035C58 256
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x016 eventObjAnimStep800EB29C: the sound call takes a leading 0.
#include "raw7.h"
extern int func_800180D8_pcpx96084(void *);
extern void func_80022FC8(int, int, int);
void eventObjAnimStep800EB29C(char *o)
{
    switch (U8(o, 3)) {
    case 0:
        switch (U8(o, 6)) {
        case 0:
            U8(o, 6)++;
            break;
        case 1:
            func_800180D8_pcpx96084(o);
            break;
        }
        break;
    case 1:
        switch (U8(o, 6)) {
        case 0:
            func_80022FC8(0, 0xe, 4);
            U8(o, 6)++;
            break;
        case 1:
            if (func_800180D8_pcpx96084(o)) U8(o, 4) = 2;
            break;
        }
        break;
    case 2:
        switch (U8(o, 6)) {
        case 0:
            func_80022FC8(0, 0xe, 0);
            U8(o, 6)++;
            break;
        case 1:
            if (func_800180D8_pcpx96084(o)) U8(o, 4) = 2;
            break;
        }
        break;
    }
}
#endif
