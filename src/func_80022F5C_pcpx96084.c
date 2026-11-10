// VERSION PCPX_96084 tomba 80022F5C 108
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct {
    short (*tab)[4];
    short *id;
} T80022F5C;
extern T80022F5C D_800A24A0[];
extern short func_80099B30(int, int, int, int, int, int, int);

short func_80022F5C_pcpx96084(int a, int b)
{
    short (*tab)[4] = D_800A24A0[a].tab;
    return func_80099B30(*D_800A24A0[a].id, tab[b][0], tab[b][1], tab[b][2], 0, tab[b][3], tab[b][3]);
}
#endif
