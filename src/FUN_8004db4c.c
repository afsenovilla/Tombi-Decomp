// FUNC 8004db4c 196 MAIN0
// MATCHING 8004db4c 196
extern int DAT_1f800398;
extern void FUN_8004dc10(int, int);

void FUN_8004db4c(int a, int idx)
{
    short *base = (short *)(DAT_1f800398 + *(short *)(DAT_1f800398 + 8));
    short *p;
    unsigned short v;

    p = (short *)((char *)base + *(short *)((char *)base + (idx << 1)));

    while (1) {
        v = *p;
        p++;
        if ((unsigned short)(v + 2) < 2) break;
        idx = (short)v;
        if (idx != -3 && idx != -7) {
            if (idx == -6) p += 2;
            else FUN_8004dc10(a, idx);
        }
    }
}
