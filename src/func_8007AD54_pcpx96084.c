// VERSION PCPX_96084 tomba 8007AD54 20
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL bitingPlantCopyParentAngle (src/bitingPlantCopyParentAngle.c): the parent field +0x8C is read as a word.
void bitingPlantCopyParentAngle(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned *)(*(char **)(p + 0x90) + 0x8c);
}
#endif
