// VERSION PCPX_96084 tomba 80023FCC 240
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Room load: for a new area loads the area file (the end address 0x1F8003E0, rounded up to 4, becomes
// D_800DE074), then the section file, and starts the loader thread (func_800235D4 in slot 2) if it is idle.
extern char *D_800B03D8[];
extern char **D_800B03F4[];
extern unsigned int D_1F8003E0;
extern unsigned int D_800DE074;
extern unsigned short D_801FC0E0;
extern short D_800E2B30;
extern void func_800240BC(char *);
extern void ThreadCreate(int, void (*)(void));
extern void func_800235D4(void);

void func_80023FCC_pcpx96084(int area, int sec, int full)
{
    if (full) {
        unsigned int v;
        func_800240BC(D_800B03D8[area]);
        v = D_1F8003E0;
        switch ((int)(v & 3)) {
        case 1:
            v++;
        case 2:
            v++;
        case 3:
            v++;
        }
        D_800DE074 = v;
    }
    func_800240BC(D_800B03F4[area][sec]);
    D_800E2B30 = 0;
    if (D_801FC0E0 == 0) ThreadCreate(2, func_800235D4);
}
#endif
