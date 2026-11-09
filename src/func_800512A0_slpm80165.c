// VERSION SLPM_80165 slpm80165 800512A0 56
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80058694_slps01144.c (func_80058694_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern volatile S8006F07C *D_8008563C_slpm80165;
extern int D_80085644_slpm80165[];

int func_800512A0_slpm80165(int a)
{
    D_8008563C_slpm80165->f4 &= ~D_80085644_slpm80165[a & 0xffff];
    return 1;
}
#endif
