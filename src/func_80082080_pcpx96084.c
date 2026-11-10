// VERSION PCPX_96084 tomba 80082080 276
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Grab test: the box test of func_80081DB0_pcpx96084 (inlined) against b; on a hit a holds b's part +0x28 (state 1).
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern short D_800DE744;
extern short D_1F8002A6;
extern char *D_800E2450;

static __inline__ int hit(char *a, char *b)
{
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 45) >= 91)
        return 0;
    if ((unsigned short)(U16(b, 0x6c) + (U16(a, 0xbc) - U16(PTR(b, 0x40), 2))) > S16(b, 0x6e))
        return 0;
    if ((unsigned short)(U16(b, 0x70) + (U16(a, 0xbe) - U16(b, 0x16))) > S16(b, 0x72))
        return 0;
    if (U16(a, 0x2e) & 1)
        D_800DE744 = S16(b, 0x6e) - U16(b, 0x6c);
    else
        D_800DE744 = -U16(b, 0x6c);
    return 1;
}

void func_80082080_pcpx96084(char *a, char *b)
{
    char pad[0];
    if (*(int *)(b + 0x94) == 0 && hit(a, b)) {
        short d;
        char *c;
        a[0x9a] = 1;
        d = D_800DE744;
        S16(a, 0xb2) = 0xc;
        S16(a, 0x7e) = 0;
        S16(a, 0xb0) = d;
        c = PTR(b, 0x28);
        c[0x69] = 1;
        {
            unsigned short f = U16(a, 0x2e);
            D_1F8002A6 = 0;
            D_800E2450 = b;
            U16(c, 0x2e) = f & 1;
        }
    }
}
#endif
