// VERSION SLPS_01144 slps01144 80016960 192
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern volatile int D_800A1380[];
extern volatile unsigned short D_800A1382;
extern volatile unsigned short D_800A1384;
extern volatile unsigned short D_800A1386 asm("D_800A1384+2");
extern unsigned short D_8009E4B6;
extern volatile unsigned short D_1F8001E8;
extern unsigned short D_1F8001F6;
extern unsigned short D_1F8001FA;
extern int func_80064B74(int);

void vblankHandler(void)
{
    D_800A1384 = *(volatile unsigned short *)D_800A1380;
    D_800A1386 = D_800A1382;
    D_800A1380[0] = func_80064B74(1);
    {
        unsigned short o0 = D_800A1384;
        unsigned short c0 = *(volatile unsigned short *)D_800A1380;
        unsigned short o1 = D_800A1386;
        unsigned short c1 = D_800A1382;
        unsigned short f = D_1F8001F6 + 1;

        D_1F8001E8++;
        D_1F8001FA = c0 & ~o0;
        D_8009E4B6 = c1 & ~o1;
        D_1F8001F6 = f;
    }
}
#endif
