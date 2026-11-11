// VERSION PCPX_96084 tomba 800A108C 104
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
extern int D_800C806C;
extern int D_800C8070;
extern void DeliverEvent(int, int);

int func_800A108C_pcpx96084(int a, int b)
{
    D_800C806C = a;
    D_800C8070 = b;
    switch (a) {
    case 33:
        DeliverEvent(0xf4000002, 769);
        break;
    case 34:
        DeliverEvent(0xf4000002, 770);
        break;
    }
    return 0;
}
#endif
