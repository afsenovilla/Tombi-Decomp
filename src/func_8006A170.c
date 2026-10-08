// FUNC 8006a170 144 MAIN0
// MATCHING 8006a170 144
extern int *D_8009820C;
extern unsigned short *D_80098210;
extern int FUN_8006bfa4();

int func_8006A170(void)
{
    int *is = D_8009820C;
    unsigned short *j = D_80098210;
    *is = -0x81;
    if (j[2] & 0x80) {
        do {
            if (FUN_8006bfa4()) return 0;
        } while (D_80098210[2] & 0x80);
    }
    D_80098210[5] |= 0x10;
    return 1;
}
