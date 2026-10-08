// FUNC 80019f98 284 MAIN0
// MATCHING 80019f98 284
extern unsigned short *DAT_1f8001d4;
extern short DAT_1f8001dc;
extern short DAT_1f8001de;
extern unsigned char DAT_1f8001ce;
extern void FUN_8001a43c(void), FUN_8001a64c(void), FUN_8001ab18(void), FUN_8001aeb4(void), FUN_8001b49c(void), FUN_8001b7cc(void), FUN_8001baf4(void), FUN_8001c364(void), FUN_8001c634(void);
void FUN_80019f98(void)
{
    short *p;
    short a, b;
    switch (DAT_1f8001d4[0x4c / 2]) {
    case 0: FUN_8001a43c(); break;
    case 1: FUN_8001a64c(); break;
    case 2: FUN_8001ab18(); break;
    case 3: FUN_8001aeb4(); break;
    case 4: FUN_8001b49c(); break;
    case 5: FUN_8001b7cc(); break;
    case 6: FUN_8001baf4(); break;
    case 7: FUN_8001c364(); break;
    case 8: FUN_8001c634();
    }
    a = DAT_1f8001dc;
    if (-1 < a && DAT_1f8001ce != 0) {
        DAT_1f8001dc = -1;
        DAT_1f8001d4[0x4c / 2] = a;
        DAT_1f8001d4[0x4e / 2] = DAT_1f8001de;
    }
}
