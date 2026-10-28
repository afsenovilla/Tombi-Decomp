// FUNC 801361e4 480 X000
// MATCHING 801361e4 480
// probably: variant-0 scene init: allocates an unlayered type 0x17 at 0xA28/-0x140, a layer 4 type 0x18 at 0xA46/-0x10C and a layer 4 type 0x17 model at 0x9EA/-0x121 (flag 8 of GAME_AREA00_EVENT_CONTROL)
extern unsigned GAME_AREA00_EVENT_CONTROL;
extern unsigned char DAT_8009cdac, DAT_8009cebf, DAT_800a60a3, DAT_800a60a2;
extern int DAT_1f800334;
extern unsigned char *FUN_80018448(void);
extern unsigned char *allocObjectLayer4(void);

void a00Variant0EventSpawn(unsigned char *o)
{
    unsigned char *a, *b, *c;
    int n;
    unsigned *fl = &GAME_AREA00_EVENT_CONTROL;
    *fl |= 8;
    if (DAT_8009cdac == 0xff) {
        o[4] = 3;
        return;
    }
    if (DAT_8009cebf != 0)
        return;
    DAT_800a60a3 = 0;
    DAT_800a60a2 = 0;
    a = FUN_80018448();
    if (a != 0) {
        a[0] = 1;
        a[2] = 0x17;
        *(int *)(a + 0x10) = 0xa280000;
        *(int *)(a + 0x14) = 0xfec00000;
        *(int *)(a + 0x18) = 0x2a60000;
        *(signed char *)(a + 0xf) = -7;
        a[0xa] = 0;
        *(short *)(a + 0x2e) = 0;
        a[0x1d] = 5;
        a[4] = 0;
        a[5] = 0;
        a[6] = 0;
    }
    b = allocObjectLayer4();
    if (b != 0) {
        b[0] = 1;
        b[2] = 0x18;
        *(int *)(b + 0x10) = 0xa460000;
        *(int *)(b + 0x14) = 0xfef40000;
        *(int *)(b + 0x18) = 0x1340000;
        *(signed char *)(b + 0xf) = -7;
        b[0xa] = 0;
        *(short *)(b + 0x2e) = 0;
        b[0x1d] = 5;
        b[4] = 0;
        b[5] = 0;
        b[6] = 0;
    }
    c = allocObjectLayer4();
    if (c != 0) {
        volatile int *pp = &DAT_1f800334;
        int base;
        n = *(int *)(*pp + 4);
        base = *pp;
        c[0xa4] = 1;
        c[0] = 1;
        c[2] = 0x17;
        *(int *)(c + 0x10) = 0x9ea0000;
        *(int *)(c + 0x14) = 0xfedf0000;
        *(int *)(c + 0x18) = 0x2b50000;
        c[0xa] = 0x11;
        *(signed char *)(c + 0xf) = -7;
        c[3] = 0;
        c[0xc] = 0;
        *(short *)(c + 0x2e) = 0;
        c[0x1d] = 5;
        c[4] = 0;
        c[5] = 0;
        c[6] = 0;
        *(int *)(c + 0xa0) = base + n;
    }
    *(unsigned char **)(o + 0x1c) = a;
    *(unsigned char **)(o + 0x20) = b;
    *(unsigned char **)(o + 0x24) = c;
    o[4] = 1;
}
