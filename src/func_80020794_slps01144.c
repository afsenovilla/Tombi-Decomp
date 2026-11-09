// VERSION SLPS_01144 slps01144 80020794 200
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern volatile unsigned short D_8009F200[];
extern volatile unsigned short D_8009F202;
extern volatile unsigned short D_8009F204;
extern volatile unsigned short D_8009F206 asm("D_8009F204+2");
extern volatile unsigned short D_800A1380[];
extern volatile unsigned short D_800A1382;
extern unsigned short D_8009F180;
extern unsigned short D_800A137A;
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8001FE;

void func_80020794(void)
{
    D_8009F204 = D_8009F200[0];
    D_8009F206 = D_8009F202;
    D_8009F200[0] = D_800A1380[0];
    D_8009F202 = D_800A1382;
    D_1F8001FC = D_8009F200[0] & ~D_8009F204;
    D_8009F180 = D_8009F202 & ~D_8009F206;
    D_1F8001FE = D_8009F204 & ~D_8009F200[0];
    D_800A137A = D_8009F206 & ~D_8009F202;
}
#endif
