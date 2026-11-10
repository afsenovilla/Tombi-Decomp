// VERSION PCPX_96084 tomba 80034214 248
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type-0x12 child of parent at (x, y, z), facing like the parent; draw word +0x1E 0x80 or 0x20 by
// func_8008A79C_pcpx96084, CLUT word 0x783E, depth -26, scratchpad word 0x1F800154.
#include "TOBJ.H"
extern TObj *func_8001A464_pcpx96084(void);
extern int func_8008A79C_pcpx96084(void);
extern int D_1F800154;

static __inline__ short getm(void)
{
    if (func_8008A79C_pcpx96084() == 1) return 0x80;
    if (func_8008A79C_pcpx96084() == 2) return 0x80;
    return 0x20;
}

void func_80034214_pcpx96084(TObj *parent, short x, short y, short z)
{
    TObj *n = func_8001A464_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->type = 0x12;
        n->animFrame = parent->animFrame;
        *(int *)((char *)n + 0x10) = x << 16;
        *(int *)((char *)n + 0x14) = y << 16;
        *(int *)((char *)n + 0x18) = z << 16;
        *(volatile unsigned short *)&n->w1e = getm();
        *(unsigned short *)((char *)n + 8) = 0x783e;
        n->b0d = 1;
        *(short *)&n->b0a = 8;
        n->subtype = 0;
        *(signed char *)&n->b0f = -26;
        n->d3c = D_1F800154;
        n->d90 = (int)parent;
        n->category |= 0x80;
    }
}
#endif
