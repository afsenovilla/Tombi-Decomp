// VERSION PCPX_96084 tomba 80087694 56
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/func_80058694_slps01144.c (func_80058694_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern volatile S8006F07C *D_8008563C_pcpx96084;
extern int D_80085644_pcpx96084[];

int func_80087694_pcpx96084(int a)
{
    D_8008563C_pcpx96084->f4 &= ~D_80085644_pcpx96084[a & 0xffff];
    return 1;
}
#endif
