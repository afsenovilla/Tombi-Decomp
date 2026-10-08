// FUNC 80126198 168 X000
// MATCHING 80126198 168
extern short FUN_80043260(void);
extern void FUN_8001f96c(int, int, int, int);

void FUN_80126198(char *o, char *p)
{
    short r = FUN_80043260();
    if (r != 0) {
        if (o[0xa6] & 2)
            o[0xa6] = o[0xa6] + 2;
        if (r == 1 && o[0xac] == r) {
            p[0] = 2;
            p[4] = 2;
            p[5] = 0;
            p[6] = 0;
            p[0x69] = 0;
            *(char **)(o + 0xe4) = p;
            o[0xac] = 2;
            FUN_8001f96c(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
        }
    }
}
