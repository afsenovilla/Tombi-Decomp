// VERSION PCPX_96084 tomba 800A0390 52
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
extern int D_800C78A0;
extern int D_800C7D20;

void func_800A0390_pcpx96084(int a)
{
    int v;
    switch (a) {
    case 0: v = 0; break;
    case 1: v = 1; break;
    default: v = 0; break;
    }
    D_800C78A0 = a;
    D_800C7D20 = v;
}
#endif
