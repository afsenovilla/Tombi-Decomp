// FUNC 800eb8f0 256 X000
extern short DAT_80114c08[];
extern unsigned char *FUN_80018448(void);

void FUN_800eb8f0(unsigned char *o, short x, short y, int z)
{
    short *p;
    int i;
    unsigned char *n;

    p = DAT_80114c08;
    z <<= 16;
    i = 0;
    do {
        n = FUN_80018448();
        if (n != 0) {
            n[0] = 1;
            n[2] = 0x1e;
            n[0xc] = o[0xc] & 0x7f;
            *(int *)(n + 0x10) = (x + *p++) << 16;
            *(int *)(n + 0x18) = z;
            *(int *)(n + 0x14) = (y + *p++) << 16;
            n[0xa] = 2;
            n[3] = i;
            *(short *)(n + 0x1e) = *(short *)(o + 0x1e);
            *(int *)(n + 0x3c) = *(int *)(o + 0x3c);
            n[0xf] = o[0xf];
        }
        i++;
    } while (i < 5);
}
