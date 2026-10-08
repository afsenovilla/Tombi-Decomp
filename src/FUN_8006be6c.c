// FUNC 8006be6c 224 MAIN0
// MATCHING 8006be6c 224
// CC gcc-2.8.1
// FLAGS -O2 -G8
extern void (*DAT_800981b0)(unsigned char *);

void FUN_8006be6c(unsigned char *o)
{
    int s = o[0x46];
    unsigned int t;
    *(int *)(o + 0x4c) = *(int *)(o + 0x4c) + 1;
    if (s == 0)
        goto tail;
    if (s == 1) {
        t = o[0x4a];
        if (t > 10) {
            o[0x49] = 2;
            o[0x46] = 0xff;
            return;
        }
        o[0x4a] = t + 1;
        return;
    }
    t = o[0x4a];
    if (t <= 10) {
        o[0x4a] = t + 1;
        return;
    }
    if (o[0x49] != 0)
        DAT_800981b0(o);
tail:
    if (**(unsigned char **)(o + 0x3c) != 0xf3) {
        **(unsigned char **)(o + 0x30) = 0xff;
        (*(unsigned char **)(o + 0x30))[1] = 0;
        o[0xe8] = 0;
        o[0x35] = 0;
    }
}
