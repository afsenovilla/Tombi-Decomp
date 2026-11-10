// VERSION PCPX_96084 tomba 80018E24 80
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct {
    unsigned short idx;
    char pad[0x32];
    int flags[1][8];
} Flags80018E24;
extern Flags80018E24 D_800DE0D8;

int func_80018E24_pcpx96084(int n)
{
    int w = n / 32;
    int b = n - w * 32;
    return D_800DE0D8.flags[D_800DE0D8.idx][w] & (1 << b);
}
#endif
