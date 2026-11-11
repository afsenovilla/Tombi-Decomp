// VERSION PCPX_96084 tomba 80060F68 156
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern short D_1F800126;
extern short D_1F80039C;
extern short D_1F8002D8;
extern int D_1F80030C;
extern int D_1F8003B0;

void func_80060F68_pcpx96084(void)
{
    if (D_1F800126 != 0) {
        int i = D_1F80039C;
        while (i != 0) i--;
        return;
    }
    D_1F80039C = D_1F8002D8;
    D_1F8003B0 = D_1F80030C;
    if (D_1F80039C != 0) {
        do {
            D_1F80030C += 4;
        } while (--D_1F8002D8 != 0);
    }
}
#endif
