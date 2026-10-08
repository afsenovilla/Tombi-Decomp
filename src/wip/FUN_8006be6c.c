// FUNC 8006be6c 224 MAIN0
extern void (*DAT_800981b0)(char *);

void FUN_8006be6c(char *o)
{
    unsigned char s = o[0x46];
    *(int *)(o + 0x4c) = *(int *)(o + 0x4c) + 1;
    if (s != 0) {
        if (s == 1) {
            if ((unsigned char)o[0x4a] > 10) {
                o[0x49] = 2;
                o[0x46] = 0xff;
                return;
            }
        } else {
            if ((unsigned char)o[0x4a] > 10) {
                if (o[0x49] != 0)
                    DAT_800981b0(o);
                goto tail;
            }
        }
        o[0x4a] = o[0x4a] + 1;
        return;
    }
tail:
    if (**(unsigned char **)(o + 0x3c) != 0xf3) {
        **(unsigned char **)(o + 0x30) = 0xff;
        *(char *)(*(char **)(o + 0x30) + 1) = 0;
        o[0xe8] = 0;
        o[0x35] = 0;
    }
}
