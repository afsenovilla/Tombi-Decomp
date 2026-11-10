// VERSION PCPX_96084 tomba 80020DD8 140
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Room change: remembers area / section / a word, sets the pending state from the per-area table and starts the load.
extern unsigned short D_800DE0D8, D_800DE0DA, D_800DE0FA;
extern unsigned short *D_800A1AA4[];
extern volatile short D_800E61DC;
extern volatile short D_800DE4A6;
extern short D_800DE4A4, D_800DE4A8, D_800E2B44;
extern void func_80023FCC_pcpx96084(int, int, int);

void func_80020DD8(short m)
{
    unsigned short area = D_800DE0D8;
    unsigned short sec = D_800DE0DA;
    short v = *(volatile unsigned short *)&D_800A1AA4[area][sec];
    D_800E61DC = 0;
    D_800DE4A6 = sec;
    D_800DE4A4 = area;
    D_800DE4A8 = D_800DE0FA;
    D_800E2B44 = v;
    func_80023FCC_pcpx96084(area, sec, m);
}
#endif
