// FUNC 8004df00 768 MAIN0
// MATCHING 8004df00 768
typedef struct { char p0[0x48]; short w48; unsigned short w4a; unsigned short w4c; char p4e[0x58 - 0x4e]; short w58; } G;
extern G *D_1f8001d4;
extern unsigned char D_1f8001ce, D_1f8001cc, D_1f8001cd;
extern short D_1f8001f4;
extern int D_1f800164;
extern char D_800B3E28[];
extern void func_8001D6A4();
extern void SetDispMask(int);
extern void FUN_8004fb68(int);
extern void FUN_8004fa80(int, int);
extern void func_800E8A14(void);
extern void func_800E8A2C(void);
extern void FUN_80016ca8(int, int, int);
extern void FUN_80016e34(int, int, int, int);
extern void ThreadCreate(int, void *);
extern void stopBgm(int);
extern void FUN_8001d63c(int);
void func_8004DF00(void)
{
    switch (D_1f8001d4->w4a) {
    case 0:
        switch (D_1f8001d4->w4c) {
        case 0:
            SetDispMask(0);
            D_1f8001ce = 0;
            FUN_8004fb68(1);
            FUN_8004fa80(1, 1);
            D_1f8001d4->w4c++;
            break;
        case 1:
            if (D_1f8001ce != 0) {
                D_1f8001d4->w4a = 2;
                D_1f8001d4->w4c = 0;
                func_800E8A14();
            }
            break;
        }
        break;
    case 1:
        SetDispMask(0);
        func_800E8A14();
        goto next;
    case 2:
        D_1f8001d4->w4a++;
        FUN_80016ca8(0xff, 0xff, 0xff);
        D_1f8001cc = 1;
        D_1f8001cd = 0x15;
        ThreadCreate(1, func_8001D6A4);
        break;
    case 3:
        if (D_1f8001cc != 0) break;
        goto next;
    case 4:
        SetDispMask(0);
        FUN_80016e34(0xff, 0xff, 0xff, 0xc);
        SetDispMask(1);
        D_1f800164 = (int)(D_800B3E28 + D_1f8001f4 * 0xc000) & 0xffffff;
        func_800E8A2C();
        D_1f8001d4->w58 = 0x78;
        D_1f8001d4->w4a++;
        break;
    case 5:
        D_1f800164 = (int)(D_800B3E28 + D_1f8001f4 * 0xc000) & 0xffffff;
        func_800E8A2C();
        if (--D_1f8001d4->w58 > 0) break;
        stopBgm(0);
        SetDispMask(0);
        FUN_80016ca8(0, 0, 0);
        D_1f8001ce = 0;
        FUN_8004fb68(2);
        FUN_8004fa80(2, 1);
    next:
        D_1f8001d4->w4a++;
        break;
    case 6:
        if (D_1f8001ce != 0) {
            D_1f8001d4->w4a++;
            FUN_8001d63c(0);
        }
        break;
    case 7:
        D_1f8001d4->w48 = 3;
        D_1f8001d4->w4a = 0;
        break;
    }
}
