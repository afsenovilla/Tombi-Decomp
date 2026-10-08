// FUNC 80045bac 352 MAIN0
// MATCHING 80045bac 352
#define U(p, o) (*(unsigned short *)((p) + (o)))
#define S(p, o) (*(short *)((p) + (o)))

int FUN_80045bac(unsigned char *a, unsigned char *b)
{
    int dy;
    short d;
    if ((unsigned short)(U(*(unsigned char **)(a + 0x44), 2) - U(*(unsigned char **)(b + 0x44), 2) + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)(U(*(unsigned char **)(a + 0x40), 2) - U(*(unsigned char **)(b + 0x40), 2) + (U(b, 0x6c) + U(a, 0x6c))) > S(b, 0x6e) + S(a, 0x6e))
        return 0;
    dy = U(a, 0x16) - U(b, 0x16);
    if ((unsigned short)(dy + (U(b, 0x70) + U(a, 0x70))) > S(a, 0x72) + S(b, 0x72))
        return 0;
    d = dy;
    if (d <= 0) {
        if (a[0x9c] & 1)
            return 0;
        S(a, 0xb0) = 0;
        d = U(b, 0x16) - (U(b, 0x70) + U(a, 0x70));
        S(a, 0x14) = 0;
        a[0x69] = 1;
        S(a, 0x7e) = 0;
        S(a, 0x16) = d;
        b[0x69] = 1;
        return 1;
    }
    if (a[0x9c] == 0)
        return 0;
    if (d < 9)
        return 3;
    S(a, 0x16) = U(b, 0x16) + ((S(b, 0x72) - U(b, 0x70)) + (S(a, 0x72) - U(a, 0x70)));
    if (S(a, 0x7e) < 0)
        S(a, 0x7e) = 0;
    return 3;
}

