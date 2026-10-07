// FUNC 80037fbc 104 MAIN0
int FUN_80037fbc(char *o, int i)
{
    if (o[0x88] == 0) {
        unsigned short *p = unsigned short *p = (unsigned short *)(o + i * 2);((unsigned short *)o)[i];
        if (*p == 0) {
            return 2;
        } else {
            int n = 0x4f;
            int *q;
            o[0x88] = 1;
            q = (int *)(o + 0x13c);
            *(short *)(o + 0x8c) = 0;
            *(short *)(o + 0x8a) = *p - 1;
            do {
                q[0x1090 / 4] = 0;
                n--;
                q--;
            } while (n >= 0);
            return 0;
        }
    }
    return 1;
}
