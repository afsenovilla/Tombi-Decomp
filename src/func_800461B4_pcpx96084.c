// VERSION PCPX_96084 tomba 800461B4 252
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Player sprite frame by pose (+0xAA, 0..9): frame number into the player extension +0x12 and the frame mode +0x02.
#include "TOBJ.H"
typedef struct { short w00, w02; char p4[0x12 - 4]; short w12; } PX;
extern PX *D_800CCCCC;

void func_800461B4_pcpx96084(TObj *o)
{
    switch (o->waa) {
    case 0:
        D_800CCCCC->w12 = 0;
        D_800CCCCC->w02 = 0;
        break;
    case 1:
        D_800CCCCC->w12 = 0x38;
        D_800CCCCC->w02 = 8;
        break;
    case 2:
        D_800CCCCC->w12 = 0x88;
        D_800CCCCC->w02 = 8;
        break;
    case 3:
        D_800CCCCC->w12 = 0xa0;
        D_800CCCCC->w02 = 8;
        break;
    case 4:
        D_800CCCCC->w12 = 0x90;
        D_800CCCCC->w02 = 2;
        break;
    case 5:
        D_800CCCCC->w12 = 0x92;
        D_800CCCCC->w02 = 2;
        break;
    case 6:
        D_800CCCCC->w12 = 0x94;
        D_800CCCCC->w02 = 2;
        break;
    case 7:
        D_800CCCCC->w12 = 0x96;
        D_800CCCCC->w02 = 2;
        break;
    case 8:
        D_800CCCCC->w12 = 0x98;
        D_800CCCCC->w02 = 2;
        break;
    case 9:
        D_800CCCCC->w12 = 0x9a;
        D_800CCCCC->w02 = 2;
        break;
    }
}
#endif
