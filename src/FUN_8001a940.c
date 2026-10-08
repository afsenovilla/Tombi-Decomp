// FUNC 8001a940 472 MAIN0
// MATCHING 8001a940 472
extern int DAT_1f800164;
extern short DAT_1f8001f4;
extern char DAT_800b3e28[];
extern short DAT_1f8001c6;
extern unsigned char DAT_1f8001cc;
extern unsigned char DAT_1f8003d0;
extern unsigned char DAT_1f8001cd;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_1f8001d3;
extern unsigned short DAT_1f8001f8;
extern void FUN_8001ca58(void);
extern void FUN_800310d0(void);
extern void FUN_800264d0(void);
extern void FUN_80039580(void);
extern void FUN_8001d36c(void);
extern void FUN_80030f38(void);
extern void FUN_8004c0dc(void);
extern void FUN_80047af0(void);
extern void FUN_80027124(void);
extern void FUN_8002af10(void);
extern void FUN_8002b020(void);
extern void FUN_80017678(void);
extern void FUN_8005042c(void);
extern void FUN_8001dbdc(void);

void FUN_8001a940(void)
{
    unsigned char c;
    DAT_1f800164 = (int)&DAT_800b3e28[DAT_1f8001f4 * 0xc000] & 0xffffff;
    if (DAT_1f8001c6 == 2) {
        if (DAT_1f8001cc == 0) {
            DAT_1f8001c6 = 0;
        } else if (DAT_1f8003d0 == 1) {
            c = DAT_1f8001cd;
            if ((c == 5 || (unsigned char)(c - 7) < 2) && (DAT_1f8001fc & 0x4008)) {
                DAT_1f8001d3 = 1;
                DAT_1f8003d0 = 0;
            }
        }
    }
    FUN_8001ca58();
    if (DAT_1f8001c6 == 0) {
        DAT_1f8001f8++;
        FUN_800310d0();
        FUN_800264d0();
        if (DAT_1f8001c6 == 0) {
            FUN_80039580();
            FUN_8001d36c();
            FUN_80030f38();
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
