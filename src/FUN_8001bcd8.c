// FUNC 8001bcd8 324 MAIN0
// MATCHING 8001bcd8 324
extern short G1f4, G1c6;
extern unsigned short G1f8;
extern unsigned int G164;
extern char D3e28[];
extern void FUN_8001ca58(void), FUN_800310d0(void), FUN_8001d36c(void), FUN_8004c0dc(void), FUN_800ec26c(void);
extern void FUN_80027124(void), FUN_8002af10(void), FUN_8002b020(void), FUN_80030f38(void);
extern void FUN_80017678(void), FUN_8005042c(void), FUN_8001dbdc(void);

void FUN_8001bcd8(void)
{
    G164 = (unsigned int)(D3e28 + G1f4 * 0xc000) & 0xffffff;
    FUN_8001ca58();
    if (G1c6 == 0) {
        G1f8 = G1f8 + 1;
        FUN_800310d0();
        if (G1c6 == 0) {
            FUN_8001d36c();
            FUN_8004c0dc();
            FUN_800ec26c();
        }
    }
    if (G1c6 != 1)
        FUN_80027124();
    if (G1c6 == 0) {
        FUN_8002af10();
        FUN_8002b020();
        if (G1c6 == 0)
            FUN_80030f38();
    }
    if (G1c6 != 2)
        FUN_8005042c();
    else
        FUN_80017678();
    FUN_8001dbdc();
}
