// FUNC 80037fbc 104 MAIN0
int FUN_80037fbc(char *o, int i)
{
    if (o[0x88] == 0) {
        unsigned short *p = (unsigned short *)(i * 2 + (int)o);
        if (*p == 0) {
            return 2;
        } else {
            int n;
            int *q;
            for (n = 0x4f, o[0x88] = 1, q = (int *)(o + 0x13c), *(short *)(o + 0x8c) = 0, *(short *)(o + 0x8a) = *p - 1; n >= 0; n--, q--)
                q[0x1090 / 4] = 0;
            return 0;
        }
    }
    return 1;
}
