// VERSION SLPS_01144 slps01144 80058694 56
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern volatile S8006F07C *D_8008563C;
extern int D_80085644[];

int func_80058694_slps01144(int a)
{
    D_8008563C->f4 &= ~D_80085644[a & 0xffff];
    return 1;
}
#endif
