// VERSION PCPX_96084 tomba 8006C0E8 160
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of weaponHitCat4Type01 (src/weaponHitCat4Type01.c, demo branch).
extern short func_8006B8A0(char *, char *);
extern void func_80017C14_pcpx96084(int, int, int, int);
extern short func_80022F5C_pcpx96084(int, int);

void func_8006C0E8_pcpx96084(char *a, char *b)
{
    char *t;
    unsigned short u;
    if (func_8006B8A0(a, b) >= 0) {
        t = b;
        if (*(unsigned short *)(b + 0x2c) != 0)
            t = *(char **)(b + 0x28);
        *(unsigned short *)(t + 0x2e) = *(unsigned short *)(a + 0x2e) & 1;
        u = *(unsigned short *)(b + 0x2c);
        t[0x68] = 1;
        t[0x6b] = u;
        func_80017C14_pcpx96084(1, *(short *)(a + 0x12), *(short *)(a + 0x16), *(short *)(a + 0x1a));
        if (a[2] == 0)
            func_80022F5C_pcpx96084(0, 5);
    }
}
#endif
