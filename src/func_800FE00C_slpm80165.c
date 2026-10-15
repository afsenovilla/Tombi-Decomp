// VERSION SLPM_80165 x00 800FE00C 108
#if defined(VERSION_SLPM_80165)
// Demo-only helper: on pad bits 0x60, player animation 15 (or 14 without 0x4000) and this object to step 2.
typedef struct { char p[0x2c]; unsigned short w2c; } P;
extern P *D_800893C8;
extern unsigned short D_1F8001FC, D_8009B5C0;
void func_800FE00C_slpm80165(unsigned char *o)
{
    if (D_1F8001FC & 0x60) {
        if (*(volatile unsigned short *)&D_8009B5C0 & 0x4000)
            D_800893C8->w2c = 15;
        else
            D_800893C8->w2c = 14;
        o[0x9c] = 0;
        *(short *)(o + 0x7e) = 0;
        o[7] = 2;
    }
}
#endif
