// FUNC 800184d8 144 MAIN0
// Best attempt: only differs in prologue order (game: addiu sp first, ours puts it after the first lh).
extern short DAT_1f800238;
extern char **DAT_1f800208;
extern unsigned short DAT_1f8001c8;

char *FUN_800184d8(void)
{
    char *r;
    char four = 4;
    char pad;
    if (DAT_1f800238 > 0) {
        char **p = DAT_1f800208;
        DAT_1f800238 = DAT_1f800238 - 1;
        DAT_1f800208 = p + 1;
        r = *p;
        r[0x1c] = four;
        if ((DAT_1f8001c8 & 1) == 0) {
            *(char **)(r + 0x40) = r + 0x10;
            *(char **)(r + 0x44) = r + 0x18;
        } else {
            *(char **)(r + 0x44) = r + 0x10;
            *(char **)(r + 0x40) = r + 0x18;
        }
        return r;
    }
    return 0;
}
