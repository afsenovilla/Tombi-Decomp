// FUNC 800183b8 144 MAIN0
// MATCHING 800183b8 144
extern short g238;
extern int *g208;
extern unsigned short g1c8;
int FUN_800183b8(void)
{
    int o;
    short n;
    char c;
    char pad;
    n = g238;
    c = 2;
    if (n > 0) {
        g238 = n - 1;
        o = *g208++;
        *(char *)(o + 0x1c) = c;
        if ((g1c8 & 1) == 0) {
            *(int *)(o + 0x40) = o + 0x10;
            *(int *)(o + 0x44) = o + 0x18;
        } else {
            *(int *)(o + 0x44) = o + 0x10;
            *(int *)(o + 0x40) = o + 0x18;
        }
        return o;
    }
    return 0;
}
