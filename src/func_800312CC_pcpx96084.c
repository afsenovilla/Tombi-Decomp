// VERSION PCPX_96084 tomba 800312CC 240
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Spawns a type-3 object at (x, y, z) with the sprite tables D_800A29C4 / D_800A2A34; the draw words +0xA0 / +0xC0
// depend on func_8008A79C_pcpx96084 (1 or 2: 0x0A / 0x8A, else 0x0A / 0x2A).
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
extern TObj *func_8001A464_pcpx96084(void);
extern int func_8008A79C_pcpx96084(void);
extern char D_800A29C4[], D_800A2A34[];

void func_800312CC_pcpx96084(short x, short y, short z)
{
    TObj *n = func_8001A464_pcpx96084();
    if (n != 0) {
        n->active = 1;
        n->type = 3;
        *(short *)&n->b0a = 5;
        *(int *)((char *)n + 0x10) = x << 16;
        *(int *)((char *)n + 0x14) = y << 16;
        *(int *)((char *)n + 0x18) = z << 16;
        *(char **)((char *)n + 0xa4) = D_800A29C4;
        n->subtype = 0;
        n->animTimer = 0;
        *(char **)((char *)n + 0xc4) = D_800A2A34;
        U16(n, 0xa0) = (func_8008A79C_pcpx96084() == 1 || func_8008A79C_pcpx96084() == 2) ? 0xa : 0xa;
        U16(n, 0xc0) = (func_8008A79C_pcpx96084() == 1 || func_8008A79C_pcpx96084() == 2) ? 0x8a : 0x2a;
        U16(n, 0xa2) = 0x781e;
    }
}
#endif
