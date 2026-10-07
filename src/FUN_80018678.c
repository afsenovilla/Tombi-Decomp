// FUNC 80018678 128 MAIN0
extern short DAT_1f80023c;
extern int *DAT_1f800214;
extern unsigned short DAT_1f8001c8;

int FUN_80018678(void)
{
    int r;
    if (DAT_1f80023c > 0) {
        int *p;
        DAT_1f80023c = DAT_1f80023c - 1;
        p = DAT_1f800214;
        DAT_1f800214 = p + 1;
        r = *p;
        if ((DAT_1f8001c8 & 1) == 0) {
            *(int *)(r + 0x40) = r + 0x10;
            *(int *)(r + 0x44) = r + 0x18;
        } else {
            *(int *)(r + 0x44) = r + 0x10;
            *(int *)(r + 0x40) = r + 0x18;
        }
    } else {
        r = 0;
    }
    return r;
}
