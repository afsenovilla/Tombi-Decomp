// FUNC 800ec5f4 284 X004
// MATCHING 800ec5f4 284
extern char *FUN_80018448(void);

void FUN_800ec5f4(char *o, int a, int b, int c)
{
    char *p = FUN_80018448();
    if (p != 0) {
        *p = 1;
        p[2] = 0x5c;
        p[3] = 9;
        *(int *)(p + 0x10) = *(int *)(o + 0x10);
        *(int *)(p + 0x14) = *(int *)(o + 0x14);
        *(int *)(p + 0x18) = *(int *)(o + 0x18);
        *(short *)(*(char **)(p + 0x40) + 2) += a;
        *(short *)(p + 0x16) += b;
        *(short *)(*(char **)(p + 0x44) + 2) += c;
        *(short *)(p + 0x1e) = *(short *)(o + 0x1e);
        *(int *)(p + 0x3c) = *(int *)(o + 0x3c);
        *(int *)(p + 0x24) = *(int *)(o + 0x24);
        *(short *)(p + 0x2c) = *(short *)(o + 0x2c);
        {
            unsigned short u = *(unsigned short *)(o + 0x2e);
            p[10] = 2;
            p[0xd] = 0x80;
            *(int *)(p + 0x8c) = 0;
            p[0x6b] = 0;
            *(signed char *)(p + 0xf) = -7;
            *(unsigned short *)(p + 0x2e) = u;
            p[0x1c] |= 0x80;
        }
    }
}
