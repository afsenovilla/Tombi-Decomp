// FUNC 8004b454 296 MAIN0
#define H(p, o) (*(unsigned short *)((char *)(p) + (o)))
#define SH(p, o) (*(short *)((char *)(p) + (o)))
#define W(p, o) (*(int *)((char *)(p) + (o)))
typedef struct E { unsigned short a, b; } E;
extern E DAT_8007b5e4[];
extern short DAT_1f80019e;
extern int DAT_1f8003c0;

void FUN_8004b454(char *a, char *b)
{
    unsigned short t1, t2;
    int d, e, w;
    unsigned v;
    char pad;
    unsigned short x;

    if ((unsigned short)(H(W(a, 0x44), 2) - H(W(b, 0x44), 2) + 0x2d) < 0x5b) {
        x = H(a, 0xe8);
        d = x - H(W(a, 0x40), 2);
        t2 = DAT_8007b5e4[*(unsigned char *)(b + 0xc)].a;
        t1 = DAT_8007b5e4[*(unsigned char *)(b + 0xc)].b;
        e = (d << 16 < 0) ? -d : d;
        w = x - (H(W(b, 0x40), 2) + t2);
        if (H(a, 0x2e) & 1)
            v = H(b, 0x6c) + e;
        else
            v = H(b, 0x6c);
        if ((unsigned short)(v + w) <= SH(b, 0x6e) + (short)e
            && (unsigned short)(H(b, 0x70) + (H(a, 0xea) - (H(b, 0x16) + t1))) <= SH(b, 0x72)) {
            *(unsigned char *)(a + 0x9e) = 3;
            SH(a, 0x7e) = 0;
            H(a, 0xb8) = t2;
            H(a, 0xba) = 0;
            *(unsigned char *)(b + 0x69) = 2;
            DAT_1f80019e = 0;
            DAT_1f8003c0 = (int)b;
        }
    }
}
