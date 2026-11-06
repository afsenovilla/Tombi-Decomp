// VERSION SLPS_01144 x04 80115D9C 280
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
extern unsigned char D_800A7C24, D_800A7C14, D_800A7C16, D_800A7C22, D_8009E4E8, D_8009EE58, D_8009E4E7, D_8009E4EA;
extern int func_80027EA8(void *, int, int);
#define S16P(a) (*(short *)(0x1F800000 + (a)))
#define S1FC (*(unsigned short *)0x1F8001FC)
static __inline__ short busy(void)
{
    if (D_800A7C24 >= 2) return 1;
    if (D_800A7C14 | D_800A7C16 | D_800A7C22 | D_8009E4E8) return 1;
    if (S1FC & 0x1000) return 0;
    return 1;
}
#define FIRE(a,b) if (func_80027EA8(o, a, b)) { D_8009EE58 = 0; D_8009E4E7 = 1; D_8009E4EA = 1; }

void func_80115D9C_slps01144(void *o)
{
    if (S16P(0x16a) >= 0x6d0 && S16P(0x172) == 0x168) {
        FIRE(1, 7)
        return;
    }
    if (busy()) return;
    if (S16P(0x16a) < 0x28a && S16P(0x172) == 0x21c) {
        FIRE(1, 8)
    }
}
#endif
