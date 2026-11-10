// VERSION PCPX_96084 tomba 800259A8 136
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Inserts a place into the 10-entry table D_800E24D0 (sorted by the first word, descending): the entries from
// the insertion point down move one place, the index is returned (-1: not in the table).
typedef struct { unsigned int a, b; } Pair800259A8;
extern Pair800259A8 D_800E24D0[];

int func_800259A8_pcpx96084(unsigned int x)
{
    int i, j;
    for (i = 0; i < 10; i++) {
        if (D_800E24D0[i].a < x) {
            for (j = 9; i < j; j--) {
                D_800E24D0[j] = D_800E24D0[j - 1];
            }
            return i;
        }
    }
    return -1;
}
#endif
