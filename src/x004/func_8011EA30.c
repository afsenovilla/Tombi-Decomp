// FUNC 8011ea30 584 X004
// MATCHING 8011ea30 584
#include "spad.h"
extern void ObjListPush_1F800218(unsigned char *), ObjListPush_1F80021C(unsigned char *),
    ObjListPush_1F800220(unsigned char *), ObjListPush_1F800224(unsigned char *),
    ObjListPush_1F800228(unsigned char *), ObjListPush_1F80022C(unsigned char *),
    ObjListPush_1F800230(unsigned char *);

#define DZ(o) (((short *)*(int *)((o) + 0x44))[1])

int func_8011EA30(unsigned char *o)
{
    int k, h;

    if (o[0] == 0)
        return 0;
    o[1] = 0;
    if ((*(short*)&SPAD.s1c8))
        k = (*(short*)&SPAD.s17e) - DZ(o);
    else
        k = DZ(o) - (*(short*)&SPAD.s17e);
    k += 0xb4;
    if ((unsigned)k >= 0x277)
        return 0;
    if ((*(short*)&SPAD.s1c8))
        k = ((*(short*)&SPAD.s17e) << 8) / DZ(o);
    else
        k = (DZ(o) << 8) / (*(short*)&SPAD.s17e);
    h = ((k * 0x140 >> 8) - 0x140) / 2;
    if ((unsigned short)(*(unsigned short *)(*(int *)(o + 0x40) + 2) - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + 0x40 + h) > h * 2 + 0x1c0)
        return 0;
    h = ((k * 0xf0 >> 8) - 0xf0) / 2;
    if ((unsigned short)(SPAD.s186 - *(unsigned short *)(o + 0x16) + 0x40 + h) > h * 2 + 0x170)
        return 0;
    o[1]++;
    switch (o[0x1c] & 0x7f) {
    case 1: ObjListPush_1F800218(o); return 1;
    case 2: ObjListPush_1F80021C(o); return 1;
    case 3: ObjListPush_1F800220(o); return 1;
    case 4: ObjListPush_1F800224(o); return 1;
    case 5: ObjListPush_1F800228(o); return 1;
    case 7: ObjListPush_1F80022C(o); return 1;
    case 8: ObjListPush_1F800230(o);
    default: return 1;
    }
}
