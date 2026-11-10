// VERSION PCPX_96084 tomba 80024484 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 relative of JP func_8001FF98_slps01144 (loadWssFile): a flat table, the flag is a RAM halfword.
extern int D_800AFDD8[];
extern unsigned short D_801FC0E0;
extern short D_800E2B30;
extern void func_800240BC(int);
extern void func_800235D4(void);
extern void func_80017818(int, void (*)(void));

void func_80024484_pcpx96084(int a)
{
    func_800240BC(D_800AFDD8[a]);
    D_800E2B30 = 0;
    if (D_801FC0E0 == 0)
        func_80017818(2, func_800235D4);
}
#endif
