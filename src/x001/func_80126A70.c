// FUNC 80126a70 300 X001
// MATCHING 80126a70 300
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define S16(p, o) (*(short *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern unsigned char DAT_1f8001a4;
extern unsigned char D_8009CECC;
extern short D_1F80019E;
extern short boxesOverlap(unsigned char *, unsigned char *);
extern void setEventStarted(int, int, int);
extern void decreaseObjectTimer(unsigned char *, int);

void func_80126A70(unsigned char *a, unsigned char *b)
{
    unsigned char *c;

    if (U8(a, 0xac) == 2)
        return;
    if (boxesOverlap(a, b) == 0)
        return;
    if (DAT_1f8001a4 != 0)
        return;
    if (a[0] & 2)
        return;
    a[0] = 2;
    if (U8(a, 0xac) == 1)
        U8(a, 0xac) = 0;
    if (S16(a, 0x98) != 1) {
        c = &D_8009CECC;
        if (*c == 0) {
            setEventStarted(0xb1, 0, 1);
            *c = 1;
        }
    }
    if (S16(PTR(b, 0x40), 2) > S16(PTR(a, 0x40), 2))
        S16(a, 0x2e) = 1;
    else
        S16(a, 0x2e) = 0;
    a[4] = 2;
    a[5] = 0;
    a[6] = 0;
    U8(a, 0xd1) = 1;
    decreaseObjectTimer(a, 1);
    D_1F80019E = 0;
}
