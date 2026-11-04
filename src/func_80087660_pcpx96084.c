// VERSION PCPX_96084 tomba 80087660 52
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/StartRCnt.c (func_80058660) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern volatile S8006F07C *D_800982C8;
extern int D_800982D0[];

int func_80087660_pcpx96084(int a)
{
    D_800982C8->f4 |= D_800982D0[a &= 0xffff];
    return a < 3;
}
#endif
