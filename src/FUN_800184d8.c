// FUNC 800184d8 144 MAIN0
extern short SCR_238;
extern int **SCR_208;
extern unsigned short SCR_1C8;

char *FUN_800184d8(void)
{
    char *o;
    char st = 4;
    if (SCR_238 < 1) {
        o = 0;
    } else {
        SCR_238--;
        o = (char *)*SCR_208++;
        o[0x1c] = st;
        if ((SCR_1C8 & 1) == 0) {
            *(char **)(o + 0x40) = o + 0x10;
            *(char **)(o + 0x44) = o + 0x18;
        } else {
            *(char **)(o + 0x44) = o + 0x10;
            *(char **)(o + 0x40) = o + 0x18;
        }
    }
    return o;
}
