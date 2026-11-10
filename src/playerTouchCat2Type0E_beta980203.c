// VERSION BETA_980203 beta980203 8005234C 380
#if defined(VERSION_BETA_980203)
// Beta version of src/playerTouchCat2Type0E.c (playerTouchCat2Type0E, PAL body): the object link field 0xe4 is at 0xd8.
#define LIST_COUNTER D_1F80019E
extern short pushObjectOutOfObjectBoxA(unsigned char *, unsigned char *);
extern void spawnImpactEffect(int, int, int, int);
extern short LIST_COUNTER;

#define U(p, o) (*(unsigned short *)((p) + (o)))
#define S(p, o) (*(short *)((p) + (o)))

static __inline__ short HIT(unsigned char *a, unsigned char *b)
{
    if ((unsigned short)(U(*(unsigned char **)(a + 0x44), 2) - U(*(unsigned char **)(b + 0x44), 2) + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)(U(*(unsigned char **)(a + 0x40), 2) - U(*(unsigned char **)(b + 0x40), 2) + (U(a, 0x6c) + U(b, 0x6c))) > S(a, 0x6e) + S(b, 0x6e))
        return 0;
    if ((unsigned short)(U(a, 0x16) - U(b, 0x16) + (U(a, 0x70) + U(b, 0x70))) > S(a, 0x72) + S(b, 0x72))
        return 0;
    return 1;
}

void playerTouchCat2Type0E(unsigned char *a, unsigned char *b)
{
    short r;
    int h;
    if (b[0] & 2) {
        h = HIT(a, b);
        if (h) {
            b[0] = 2;
            b[4] = 2;
            b[5] = 7;
        }
    } else {
        r = pushObjectOutOfObjectBoxA(a, b);
        if (r != 0 && r == 1 && a[0xac] == 1 && b[0] != 3) {
            b[0] = 6;
            b[4] = 2;
            b[5] = 0;
            *(unsigned char **)(a + 0xd8) = b;
            a[0xac] = 2;
            spawnImpactEffect(2, S(a, 0x12), S(a, 0x16), S(a, 0x1a));
            LIST_COUNTER = 0;
        }
    }
}
#endif
