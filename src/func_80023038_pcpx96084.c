// VERSION PCPX_96084 tomba 80023038 108
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// One of the four calls of func_80099B30 through the table D_800A24A0 (func_80022F5C_pcpx96084.c).
typedef struct {
    short (*tab)[4];
    short *id;
} T80023038;
extern T80023038 D_800A24A0[];
extern short func_80099B30(int, int, int, int, int, int, int);

short func_80023038_pcpx96084(int a, int b, short c)
{
    short (*tab)[4] = D_800A24A0[a].tab;
    return func_80099B30(*D_800A24A0[a].id, tab[b][0], tab[b][1], tab[b][2], 0, c, c);
}
#endif
