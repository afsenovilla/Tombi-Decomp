// FUNC 80026e0c 320 MAIN0
extern unsigned char n;
extern unsigned char arrA[];
extern unsigned char arrB[];
unsigned int FUN_80026e0c(unsigned int p, int d)
{
    int i = 0;
    unsigned char b;
    unsigned int u;
    if (n != 0) {
        do {
            if (arrB[i] == p) {
                if (d == -1) {
                    arrA[p] = 0;
                    for (; i < (int)(n - 1); i++) arrB[i] = arrB[i + 1];
                } else {
                    b = arrA[p];
                    arrA[p] = b - d;
                    u = (b - d) & 0xff;
                    if (u != 0) return u;
                    arrA[p] = 0;
                    for (; i < (int)(n - 1); i++) arrB[i] = arrB[i + 1];
                }
                n = n - 1;
                return 0;
            }
            i++;
        } while (i < (int)(unsigned int)n);
    }
    return 0xffffffff;
}
