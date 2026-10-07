// FUNC 800eea3c 64 X000
void FUN_800eea3c(char *o)
{
    short s = *(short *)(o + 0xb0);
    int v = s;
    unsigned short r;
    if (s < 0) {
        r = s * 4 + 0x100;
    } else {
        r = v << 2;
        if (v < 1) {
            *(short *)(o + 0xb6) = 0;
            return;
        }
    }
    *(short *)(o + 0xb6) = r & 0xff;
}
