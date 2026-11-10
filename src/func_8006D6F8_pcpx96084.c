// VERSION PCPX_96084 tomba 8006D6F8 364
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of playerTouchCat2Type0E (src/playerTouchCat2Type0E.c): state byte +0xA4, link +0xB8, an area flag call in place of the list counter reset.
extern short func_80069700(unsigned char *, unsigned char *);
extern void func_80017C14_pcpx96084(int, int, int, int);
extern void func_80018E74_pcpx96084(int);

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

void func_8006D6F8_pcpx96084(unsigned char *a, unsigned char *b)
{
    short r;
    int h;
    if (b[0] & 2) {
        h = HIT(a, b);
        if (h) {
            b[0] = 2;
            b[4] = 2;
            b[5] = 2;
        }
    } else {
        r = func_80069700(a, b);
        if (r != 0 && r == 1 && a[0xa4] == 1) {
            b[0] = 6;
            b[4] = 2;
            b[5] = 0;
            *(unsigned char **)(a + 0xb8) = b;
            a[0xa4] = 2;
            func_80017C14_pcpx96084(2, S(a, 0x12), S(a, 0x16), S(a, 0x1a));
            func_80018E74_pcpx96084(b[0x6b]);
        }
    }
}
#endif
