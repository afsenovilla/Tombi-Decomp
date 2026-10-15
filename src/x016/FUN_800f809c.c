// FUNC 800f809c 148 X016
// MATCHING 800f809c 148
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(void *, int);
static char DAT_801152e8[36] = {
    0, 0, 0, -2, -4, -6, -8, -0xc, -0x10, -0x14, -0x18, -0x1c, -0x20, -0x24, -0x28, -0x2c,
    -0x30, 0, 0, 0, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0xc,
    8, 4,
};
extern char LAB_80010c50[];

void FUN_800f809c(char *o)
{
    unsigned char b;
    short *h;
    short d; int c; int p;
    FUN_8001e5f4(0x1c, 0x7f);
    o[0x9c] = 0;
    o[0xaa] = 0;
    *(char **)(o + 0x24) = LAB_80010c50;
    FUN_8001fe94(o, 1);
    c = *(unsigned short *)(o + 0x2e) & 1;
    h = *(short **)(o + 0x40);
    p = h[1];
    if (c) d = p - 12; else d = p + 12;
    h[1] = d;
    *(int *)(o + 0x88) = 0;
    b = DAT_801152e8[*(short *)(o + 0xb0)];
    o[6]++;
    *(unsigned int *)(o + 0x8c) = b;
}
