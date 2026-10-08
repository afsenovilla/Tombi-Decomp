// FUNC 80111a50 144 X009
// MATCHING 80111a50 144
extern int DAT_800a6048, DAT_800a604c, DAT_800a6050;
extern unsigned char *FUN_800182ac(void);
void FUN_80111a50(unsigned char a, unsigned char b, unsigned char c)
{
    unsigned char *q = FUN_800182ac();
    if (q != 0) {
        q[0] = 1;
        q[2] = 10;
        q[3] = a;
        q[0xc] = b;
        *(int *)(q + 0x10) = DAT_800a6048;
        *(int *)(q + 0x14) = DAT_800a604c;
        *(int *)(q + 0x18) = DAT_800a6050;
        q[5] = c;
    }
}
