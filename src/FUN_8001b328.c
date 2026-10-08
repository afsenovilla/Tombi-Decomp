// FUNC 8001b328 372 MAIN0
// MATCHING 8001b328 372
typedef struct { char p[0x4e]; unsigned short w; } O;
extern unsigned short G1f8;
extern short G1f4, D45ea;
extern unsigned int G164;
extern O *G1d4;
extern char D3e28[];
extern int FUN_800efaa4(void), FUN_800e9888(void), FUN_800edd28(void);
extern void FUN_800efd6c(void), FUN_800ea37c(void), FUN_800edf08(void), FUN_8005f060(int), FUN_8001dbdc(void);

void FUN_8001b328(void)
{
    int r;
    G1f8 = G1f8 + 1;
    G164 = (unsigned int)(D3e28 + G1f4 * 0xc000) & 0xffffff;
    switch (D45ea) {
    case 0: case 1: case 2: case 3: case 6:
        r = FUN_800efaa4();
        if (r == 0) FUN_800efd6c();
        break;
    case 4: case 5:
        r = FUN_800e9888();
        if (r == 0) FUN_800ea37c();
        break;
    case 7:
        r = FUN_800edd28();
        if (r == 0) FUN_800edf08();
        break;
    }
    if (D45ea == 5) {
        if (r == 1) {
            G1d4->w = 7;
            goto end;
        }
    } else if (r == 2) {
        FUN_8005f060(0);
        G1d4->w = 1;
        goto end;
    }
    if (r != 0)
        G1d4->w = G1d4->w + 1;
end:
    FUN_8001dbdc();
}
