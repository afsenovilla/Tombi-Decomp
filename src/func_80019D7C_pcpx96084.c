// VERSION PCPX_96084 tomba 80019D7C 248
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// The layer-4 pool: 96 objects of 0xD8 bytes, category 4; free list pointer at scratchpad 0x2F0, lists at 0x310 /
// 0x3B4; then the 20 effect slots of 0x8C bytes at D_800E622C are marked free (-1).
extern int *D_1F8002F0;
extern short D_1F8002CA;
extern char *D_1F800310;
extern char *D_1F8003B4;
extern short D_1F80039E;
extern short D_1F8002DA;
extern unsigned char D_800F7948[];
extern unsigned char D_800FC970[];
extern unsigned char D_800EBDB8[];
typedef struct { short s0; char pad[0x8a]; } Slot;
extern Slot D_800E622C[];
extern void memset(void *, int, int);

void func_80019D7C_pcpx96084(void)
{
    int i;
    unsigned char *p;
    unsigned char *q;
    i = 0;
    p = D_800F7948;
    for (; i < 96; i++) {
        memset(p, 0, 0xd8);
        p += 0xd8;
    }
    q = D_800FC970;
    D_1F8002F0 = (int *)D_800EBDB8;
    for (i = 0; i < 96; i++) {
        q[0x1c] = 4;
        *--D_1F8002F0 = (int)q;
        q -= 0xd8;
    }
    D_1F8002CA = 96;
    D_1F800310 = (char *)D_800F7948;
    D_1F8003B4 = (char *)D_800F7948;
    D_1F80039E = 0;
    D_1F8002DA = 0;
    for (i = 19; i >= 0; i--) D_800E622C[i].s0 = -1;
}
#endif
