// FUNC 80026c50 444 MAIN0
extern unsigned char P797b4[], D97b4;
extern unsigned char D0a4[], D1a4[];
extern unsigned short D2a4, D2a6;
extern void FUN_8004d620(unsigned int, int), SfxPlay(int);

unsigned char FUN_80026c50(unsigned int a, char b, int c)
{
    unsigned char *p;
    int j, i;
    unsigned int v;
    int n;
    char pad[8];
    v = D97b4;
    if (v != 0xff) {
        p = P797b4;
        j = 0;
        do {
            if (*p == a && !(P797b4[j + 1] > D0a4[a]))
                return D0a4[a];
            p += 2;
            j += 2;
        } while (*p != 0xff);
    }
    if (c != 0)
        FUN_8004d620(a, 0);
    i = 0;
    n = D2a4;
    if (n > 0) {
        do {
            if (D1a4[i++] == a) {
                D0a4[a] = D0a4[a] + b;
                SfxPlay(10);
                goto done;
            }
        } while (i < D2a4);
    }
    i = D2a4 - 1;
    while (i >= 0) {
        D1a4[i + 1] = D1a4[i];
        i--;
    }
    D1a4[0] = a;
    D0a4[a] = b;
    D2a4 = D2a4 + 1;
    SfxPlay(10);
    D2a6 = D2a6 | 0x8000;
done:
    return D0a4[a];
}
