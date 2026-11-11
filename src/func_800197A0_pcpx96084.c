// VERSION PCPX_96084 tomba 800197A0 368
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL resetDrawLists (src/resetDrawLists.c): nine lists, pointers at 0x1F800304.. / 0x1F8003A8..,
// counts at 0x1F800398.. / 0x1F8002D4...
extern char D_800F77C8, D_800F55AC, D_800F55B8, D_800F7948, D_800F7750, D_800FDEE0, D_800F7790, D_800FCB10, D_800F546C;

void func_800197A0_pcpx96084(void)
{
    *(void **)0x1f800304 = &D_800F77C8;
    *(void **)0x1f8003a8 = &D_800F77C8;
    *(void **)0x1f800308 = &D_800F55AC;
    *(void **)0x1f8003ac = &D_800F55AC;
    *(void **)0x1f80030c = &D_800F55B8;
    *(void **)0x1f8003b0 = &D_800F55B8;
    *(void **)0x1f800310 = &D_800F7948;
    *(void **)0x1f8003b4 = &D_800F7948;
    *(void **)0x1f800314 = &D_800F7750;
    *(void **)0x1f8003b8 = &D_800F7750;
    *(void **)0x1f8003cc = &D_800FDEE0;
    *(void **)0x1f8003c8 = &D_800FDEE0;
    *(void **)0x1f800318 = &D_800F7790;
    *(void **)0x1f8003bc = &D_800F7790;
    *(void **)0x1f80031c = &D_800FCB10;
    *(void **)0x1f8003c0 = &D_800FCB10;
    *(short *)0x1f800398 = 0;
    *(short *)0x1f8002d4 = 0;
    *(short *)0x1f80039a = 0;
    *(short *)0x1f8002d6 = 0;
    *(short *)0x1f80039c = 0;
    *(short *)0x1f8002d8 = 0;
    *(short *)0x1f80039e = 0;
    *(short *)0x1f8002da = 0;
    *(short *)0x1f8003a0 = 0;
    *(short *)0x1f8002dc = 0;
    *(short *)0x1f8003d2 = 0;
    *(short *)0x1f8003d0 = 0;
    *(short *)0x1f8003a2 = 0;
    *(short *)0x1f8002de = 0;
    *(short *)0x1f8003a4 = 0;
    *(short *)0x1f8002e0 = 0;
    *(void **)0x1f800320 = &D_800F546C;
    *(void **)0x1f8003c4 = &D_800F546C;
    *(short *)0x1f8003a6 = 0;
    *(short *)0x1f8002e2 = 0;
}
#endif
