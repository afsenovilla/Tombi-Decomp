// VERSION PCPX_96084 tomba 8001B3E4 156
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Rotates the negated position at +0x40 by the matrix at +0x20 (ApplyMatrixLV in the scratchpad) and keeps the
// integer parts at +0x14/+0x18/+0x1C.
extern int D_1F800000, D_1F800004, D_1F800008;
extern short D_1F800012, D_1F800016, D_1F80001A;
extern void ApplyMatrixLV(void *m, void *v0, void *v1);

void func_8001B3E4_pcpx96084(char *o)
{
    D_1F800000 = -*(int *)(o + 0x40);
    D_1F800004 = -*(int *)(o + 0x44);
    D_1F800008 = -*(int *)(o + 0x48);
    ApplyMatrixLV(o + 0x20, (void *)0x1F800000, (void *)0x1F800010);
    *(int *)(o + 0x14) = D_1F800012;
    *(int *)(o + 0x18) = D_1F800016;
    *(int *)(o + 0x1C) = D_1F80001A;
}
#endif
