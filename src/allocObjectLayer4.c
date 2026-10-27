// FUNC 800184d8 144 MAIN0
// MATCHING 800184d8 144
// Best attempt: only differs in prologue order (game: addiu sp first, ours puts it after the first lh).
extern short FREE_OBJECT_COUNT;
extern char **FREE_OBJECTS;
extern unsigned short DAT_1f8001c8;

char *allocObjectLayer4(void)
{
    char *r;
    char four = 4;
    char pad;
    if (FREE_OBJECT_COUNT > 0) {
        char **p = FREE_OBJECTS;
        FREE_OBJECT_COUNT = FREE_OBJECT_COUNT - 1;
        FREE_OBJECTS = p + 1;
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
