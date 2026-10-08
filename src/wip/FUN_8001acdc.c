// FUNC 8001acdc 472 MAIN0
extern short DAT_1f8001f4, DAT_1f8001c6;
extern unsigned char DAT_1f8001cc;
extern unsigned DAT_1f800164;
extern unsigned short DAT_1f8001f8;
extern unsigned char DAT_8009cda3[];
extern unsigned short *DAT_1f8001d4[];
extern unsigned short DAT_1f8003b8, DAT_1f8003ba;
extern short DAT_800a45ea, DAT_800a45ec, DAT_800a45ee;
extern unsigned char DAT_800a45d8;
extern char DAT_800b3e28[];
extern void FUN_8001ca58(void), FUN_800310d0(void), FUN_800264d0(void), FUN_80039580(void), FUN_80030f38(void);
extern void FUN_8001d36c(void), FUN_8004c0dc(void), FUN_80047af0(void), FUN_80027124(void), FUN_8002af10(void);
extern void FUN_8002b020(void), FUN_80017678(void), FUN_8005042c(void), FUN_8001dbdc(void);

void FUN_8001acdc(void)
{
    unsigned short *q;
    unsigned char *g;
    DAT_1f800164 = (unsigned)(DAT_800b3e28 + DAT_1f8001f4 * 0xc000) & 0xffffff;
    if (DAT_1f8001c6 == 2 && DAT_1f8001cc == 0)
        DAT_1f8001c6 = 0;
    FUN_8001ca58();
    if (DAT_8009cda3[0] == 0xff) {
        unsigned short x, y;
        q = DAT_1f8001d4[0];
        DAT_8009cda3[0] = 0;
        x = q[0x26];
        y = q[0x27];
        DAT_800a45ea = 6;
        DAT_800a45ec = 0;
        DAT_800a45ee = 0;
        DAT_800a45d8 = 0;
        q[0x26] = 3;
        q[0x27] = 0;
        DAT_1f8003b8 = x;
        DAT_1f8003ba = y;
    }
    if (DAT_1f8001c6 == 0) {
        DAT_1f8001f8 = DAT_1f8001f8 + 1;
        FUN_800310d0();
        FUN_800264d0();
        if (DAT_1f8001c6 == 0) {
            FUN_80039580();
            FUN_80030f38();
            FUN_8001d36c();
            FUN_8004c0dc();
            FUN_80047af0();
        }
    }
    if (DAT_1f8001c6 != 1)
        FUN_80027124();
    if (DAT_1f8001c6 == 0) {
        FUN_8002af10();
        FUN_8002b020();
    }
    if (DAT_1f8001c6 != 2)
        FUN_8005042c();
    else
        FUN_80017678();
    FUN_8001dbdc();
}
