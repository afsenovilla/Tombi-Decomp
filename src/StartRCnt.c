// FUNC 8006f07c 48 MAIN0
// MATCHING 8006f07c 48
// VERSION SLPS_01144 slps01144 80058660 52
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern volatile S8006F07C *D_800982C8;
extern int D_800982D0[];

int func_80058660(int a)
{
    D_800982C8->f4 |= D_800982D0[a &= 0xffff];
    return a < 3;
}
#endif
#else
typedef struct {
    int f0;
    int f4;
} S8006F07C;

extern S8006F07C *D_800982C8;
extern int D_800982D0[];

int StartRCnt(int a)
{
    D_800982C8->f4 |= D_800982D0[a &= 0xffff];
    return a < 3;
}
#endif
