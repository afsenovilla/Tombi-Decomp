// FUNC 800efd10 92 D0SPSUB
// MATCHING 800efd10 92
extern unsigned char D_801086F0[];
extern unsigned char D_8009CDA3[];

int subScreenCountEvents(void)
{
    int last = 0;
    int i;
    for (i = 0; i < 0x100; i++) {
        if (D_801086F0[i] != 0xFF && D_8009CDA3[D_801086F0[i]] != 0) last = i;
    }
    return last + 1;
}
