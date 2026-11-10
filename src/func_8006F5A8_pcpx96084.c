// VERSION PCPX_96084 tomba 8006F5A8 236
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of JP pushObjectOutOfCat4Type01 (src/pushObjectOutOfCat4Type01.c): only the result-2 hand-over, list counter at 0x1F8002A6.
extern short func_8006EFB4(void);
extern short func_8006F164_pcpx96084(unsigned char *, unsigned char *);
#define LIST_COUNTER D_1F8002A6
extern short LIST_COUNTER;

void func_8006F5A8_pcpx96084(unsigned char *a, unsigned char *b)
{
    short r;
    unsigned char *c;
    unsigned x;
    if (*(int *)(b + 0x94) == 0) {
        r = func_8006EFB4();
        if (r != 2)
            return;
        if (a[0x68] == 0)
            return;
        x = *(unsigned short *)(a + 0x2e) & 1;
        c = *(unsigned char **)(b + 0x28);
        goto tail;
    }
    r = func_8006F164_pcpx96084(a, b);
    if (r != 2)
        return;
    if (a[0x68] == 0)
        return;
    c = b;
    if (*(unsigned short *)(c + 0x2c) == 0)
        return;
    x = *(unsigned short *)(a + 0x2e) & 1;
    c = *(unsigned char **)(c + 0x28);
tail:
    *(unsigned short *)(c + 0x2e) = x;
    c[0x6b] = *(unsigned short *)(b + 0x2c);
    c[0x68] = a[0x68];
    a[0x68] = 0;
    LIST_COUNTER = 0;
}
#endif
