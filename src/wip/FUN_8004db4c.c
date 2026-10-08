// FUNC 8004db4c 196 MAIN0
extern int DAT_1f800398;
extern void FUN_8004dc10(int);

void FUN_8004db4c(int a, int idx)
{
    int base = DAT_1f800398 + *(short *)(DAT_1f800398 + 8);
    short *p = (short *)(base + *(short *)(base + idx * 2));
    short v;
    while (1) {
        v = *p;
        p++;
        if ((unsigned short)(v + 2) < 2) break;
        if (v != -3 && v != -7) {
            if (v == -6) p += 2;
            else FUN_8004dc10(a);
        }
    }
}
