// VERSION PCPX_96084 tomba 80025A94 220
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Inventory add: n more of item (counts at 0x800DE320); a new item is appended to the list at 0x800DE3A0
// (length at 0x800DE31E). Returns the new count.
typedef struct { unsigned short n; unsigned char count[0x80]; unsigned char list[0x80]; } INV;
extern INV D_800DE31E;
extern void func_800360C0(void);

unsigned char func_80025A94_pcpx96084(int item, int n)
{
    int i;
    func_800360C0();
    for (i = 0; i < D_800DE31E.n; i++) {
        if (D_800DE31E.list[i] == item) {
            D_800DE31E.count[item] += n;
            return D_800DE31E.count[item];
        }
    }
    D_800DE31E.list[i] = item;
    D_800DE31E.count[item] = n;
    D_800DE31E.n++;
    return D_800DE31E.count[item];
}
#endif
