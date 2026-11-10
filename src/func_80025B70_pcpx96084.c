// VERSION PCPX_96084 tomba 80025B70 296
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of removeItemFromInventory (src/removeItemFromInventory.c, the SLPM-80165 body): qty -1 removes the
// slot, else the count drops by qty and an empty item leaves the list. Inventory block as in func_80025A94_pcpx96084.
typedef struct { unsigned short n; unsigned char count[0x80]; unsigned char list[0x80]; } INV;
extern INV D_800DE31E;

int func_80025B70_pcpx96084(int id, int qty)
{
    int i;
    char pad[0];
    for (i = 0; i < D_800DE31E.n; i++) {
        if (D_800DE31E.list[i] == id) {
            if (qty == -1) {
                while (i < D_800DE31E.n - 1) {
                    D_800DE31E.list[i] = D_800DE31E.list[i + 1];
                    i++;
                }
                D_800DE31E.n -= 1;
                return 0;
            }
            D_800DE31E.count[id] = D_800DE31E.count[id] - qty;
            if (D_800DE31E.count[id] == 0) {
                while (i < D_800DE31E.n - 1) {
                    D_800DE31E.list[i] = D_800DE31E.list[i + 1];
                    i++;
                }
                D_800DE31E.n -= 1;
                return 0;
            }
            return D_800DE31E.count[id];
        }
    }
    return -1;
}
#endif
