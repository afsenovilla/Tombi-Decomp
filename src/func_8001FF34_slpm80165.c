// VERSION SLPM_80165 slpm80165 8001FF34 200
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updatePadInput (src/updatePadInput_beta980203.c): both pads come from the words D_8009FE80/82
// (filled elsewhere, no readPadPort calls) and there is no pad vibration; pressed / released edges of both pads.
extern volatile unsigned short D_8009B5C0[];
extern volatile unsigned short D_8009B5C2;
extern volatile unsigned short D_8009B5C4, D_8009B5C6;
extern volatile unsigned short D_8009FE80[];
extern volatile unsigned short D_8009FE82;
extern unsigned short D_1F8001FC, D_1F8001FE, D_8009B480, D_8009FE7A;

void func_8001FF34(void)
{
    D_8009B5C4 = D_8009B5C0[0];
    D_8009B5C6 = D_8009B5C2;
    D_8009B5C0[0] = D_8009FE80[0];
    D_8009B5C2 = D_8009FE82;
    D_1F8001FC = ~D_8009B5C4 & D_8009B5C0[0];
    D_8009B480 = ~D_8009B5C6 & D_8009B5C2;
    D_1F8001FE = ~D_8009B5C0[0] & D_8009B5C4;
    D_8009FE7A = ~D_8009B5C2 & D_8009B5C6;
}
#endif
