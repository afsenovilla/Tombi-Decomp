// VERSION PCPX_96084 tomba 800217F0 292
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Item slot i pick-up: state 3 (first item: flash, sound 1, a type-0x0D effect) becomes 1, state 4 (sound 3)
// becomes 2; each passes its message (D_800A2198[i]) to func_8002598C_pcpx96084.
#include "TOBJ.H"
typedef struct { short a, b; } MSG2;
extern unsigned char D_800DE21C[];
extern short D_800A20D0[];
extern MSG2 D_800A2198[];
extern void func_80021928_pcpx96084(int, int, int);
extern void func_80022F5C_pcpx96084(int, int);
extern TObj *func_8001A464_pcpx96084(void);
extern void func_8002598C_pcpx96084(int);

void func_800217F0_pcpx96084(int i)
{
    int st;
    switch (D_800DE21C[i]) {
    case 3:
        if (D_800A20D0[i] != 0) {
            TObj *n;
            func_80021928_pcpx96084(i, 0, 0x3c);
            func_80022F5C_pcpx96084(1, 1);
            n = func_8001A464_pcpx96084();
            if (n != 0) {
                n->active = 1;
                n->type = 0xd;
            }
        }
        func_8002598C_pcpx96084(D_800A2198[i].a);
        st = 1;
        break;
    case 4:
        if (D_800A20D0[i] != 0) {
            func_80021928_pcpx96084(i, 1, 1);
            func_80022F5C_pcpx96084(1, 3);
        }
        func_8002598C_pcpx96084(D_800A2198[i].b);
        st = 2;
        break;
    case 0:
    case 1:
    case 2:
    default:
        return;
    }
    D_800DE21C[i] = st;
}
#endif
