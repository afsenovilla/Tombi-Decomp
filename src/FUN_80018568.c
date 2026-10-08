// FUNC 80018568 144 MAIN0
// MATCHING 80018568 144
extern short DAT_1f800238;
extern char **DAT_1f800208;
extern unsigned short DAT_1f8001c8;

char *FUN_80018568(void)
{
    char *p;
    char c = 5;
    short n = DAT_1f800238;
    if (n > 0) {
        DAT_1f800238 = n - 1;
        p = *DAT_1f800208++;
        p[0x1c] = c;
        if ((DAT_1f8001c8 & 1) == 0) {
            *(char **)(p + 0x40) = p + 0x10;
            *(char **)(p + 0x44) = p + 0x18;
        } else {
            *(char **)(p + 0x44) = p + 0x10;
            *(char **)(p + 0x40) = p + 0x18;
        }
        return p;
    }
    return 0;
}
