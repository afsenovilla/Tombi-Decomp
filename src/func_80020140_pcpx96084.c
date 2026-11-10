// VERSION PCPX_96084 tomba 80020140 172
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Room entry: the same area and section as the last load only restarts the room (func_80020E64_pcpx96084);
// otherwise func_80020DD8 loads it (argument 1 for a new area or the first entry, 0 for a new section).
extern unsigned char D_800DE0E5;
extern unsigned short D_800DE0D8, D_800DE0DA, D_800DE4A4, D_800DE4A6;
extern int D_800E2B00;
extern unsigned short *D_800DE0B0;
extern void func_80020E64_pcpx96084(void);
extern void func_80020DD8(int);

void func_80020140_pcpx96084(void)
{
    int m;
    unsigned char *p = &D_800DE0E5;
    m = 1;
    if (*p == 0) {
        *p = 1;
        goto load;
    }
    if (D_800DE0D8 == D_800DE4A4) {
        m = 0;
        if (D_800DE0DA == D_800DE4A6) {
            func_80020E64_pcpx96084();
            return;
        }
    }
load:
    func_80020DD8(m);
    D_800E2B00 = 0;
    D_800DE0B0[0x4e / 2]++;
}
#endif
