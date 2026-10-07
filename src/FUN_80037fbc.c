// FUNC 80037fbc 104 MAIN0
typedef struct O { unsigned short w[68]; unsigned char st; char p; short a; short b; char p2[0x13c - 0x8e]; int tab[0]; } O;
int FUN_80037fbc(char *o, int i)
{
    int r = 1;
    if (o[0x88] == 0) {
        unsigned short *p = (unsigned short *)(o + i * 2);
        if (*p == 0) {
            r = 2;
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
            r = 0;
        }
    }
    return r;
}
