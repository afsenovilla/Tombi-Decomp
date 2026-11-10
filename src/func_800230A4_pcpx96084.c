// VERSION PCPX_96084 tomba 800230A4 112
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// One of the four calls of func_80099B30 through the table D_800A24A0 (func_80022F5C_pcpx96084.c).
typedef struct {
    short (*tab)[4];
    short *id;
} T800230A4;
extern T800230A4 D_800A24A0[];
extern short func_80099B30(int, int, int, int, int, int, int);

short func_800230A4_pcpx96084(int a, int b, short c, short d)
{
    short (*tab)[4] = D_800A24A0[a].tab;
    return func_80099B30(*D_800A24A0[a].id, tab[b][0], tab[b][1], c, 0, d, d);
}
#endif
