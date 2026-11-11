// VERSION PCPX_96084 tomba 80081F78 264
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Hang test: the box test of func_80081CF8_pcpx96084 (inlined) against b; on a hit a hangs on b (state 4).
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
    if ((unsigned short)(U16(a, 0xbe) - U16(b, 0x16) + 8) >= 17)
        return 0;
    if (U16(a, 0x2e) & 1)
        D_800DE744 = S16(b, 0x6e) - U16(b, 0x6c);
    else
        D_800DE744 = -U16(b, 0x6c);
    return 1;
}

void func_80081F78_pcpx96084(char *a, char *b)
{
    char pad[0];
    if (b[3] == 0 && hit(a, b)) {
        short d;
        a[0x9a] = 4;
        d = D_800DE744;
        S16(a, 0xb2) = -1;
        S16(a, 0x7e) = 0;
        D_1F8002A6 = 0;
        S16(a, 0xb0) = d;
        b[0x6a] = 1;
        {
            unsigned short f = U16(a, 0x2e);
            D_800E2450 = b;
            U16(b, 0x2e) = f & 1;
        }
    }
}
#endif
