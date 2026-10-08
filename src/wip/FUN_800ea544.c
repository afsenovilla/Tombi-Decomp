// FUNC 800ea544 312 X000
extern unsigned char *FUN_80018448(void);
extern signed char DAT_80114a80[];

void FUN_800ea544(unsigned char *a, short x, short y, short z)
{
    unsigned char *o;
    signed char *p;
    int i;
    i = 0;
    do {
        o = FUN_80018448();
        if (o != 0) {
            p = DAT_80114a80 + i * 4;
            o[0] = 2;
            o[2] = 5;
            *(short *)(o + 0x2e) = 0;
            *(int *)(o + 0x10) = (x + p[0]) << 16;
            *(int *)(o + 0x14) = (y + p[1]) << 16;
            *(int *)(o + 0x18) = (z + p[2]) << 16;
            o[0xf] = 0xfe;
            o[0xd] = 0;
            o[0xa] = 2;
            o[3] = i;
            *(int *)(o + 0x8c) = 0x1000;
            *(short *)(o + 0xb8) = (signed char)((unsigned char *)DAT_80114a80)[k + 3];
            *(int *)(o + 0x3c) = *(int *)(a + 0x3c);
            *(unsigned short *)(o + 0x1e) = *(unsigned short *)(a + 0x1e);
            o[0x1d] = a[0x1d];
        }
        i++;
    } while (i < 6);
}
