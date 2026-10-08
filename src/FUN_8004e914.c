// FUNC 8004e914 500 MAIN0
// MATCHING 8004e914 500
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; short w4e; } S;
extern S *DAT_1f8001d4;
extern short DAT_1f8001f4;
extern int DAT_1f800164;
extern char DAT_800b3e28[];
extern unsigned char DAT_1f8001ce;
extern void FUN_8001f110(int);
extern void FUN_8005f060(int);
extern void FUN_8004fb68(int);
extern void FUN_8004fa80(int, int);
extern void FUN_800e827c(void);
extern void FUN_8001eff8(int);
extern int FUN_800e8284(void);
extern void FUN_800e8618(void);
extern void FUN_800e8700(void);

void FUN_8004e914(void)
{
    S *g;
    DAT_1f800164 = (DAT_1f8001f4 * 0xc000 + (int)DAT_800b3e28) & 0xffffff;
    switch (DAT_1f8001d4->w4a) {
    case 0:
        FUN_8001f110(0);
        FUN_8005f060(0);
        DAT_1f8001ce = 0;
        FUN_8004fb68(3);
        FUN_8004fa80(8, 1);
        DAT_1f8001d4->w4a++;
        break;
    case 1:
        if (DAT_1f8001ce != 0) {
            FUN_8005f060(1);
            FUN_800e827c();
            FUN_8001eff8(0);
            g = DAT_1f8001d4;
            g->w4c = 0;
            g->w4a++;
        }
        break;
    case 2:
        switch (DAT_1f8001d4->w4c) {
        case 0:
            DAT_1f8001d4->w4e = 0;
            DAT_1f8001d4->w4c++;
        case 1:
            if (FUN_800e8284() != 0)
                DAT_1f8001d4->w4a++;
            break;
        case 2:
            FUN_800e8618();
            break;
        case 3:
            FUN_800e8700();
            break;
        }
        break;
    case 3:
        FUN_8001f110(0);
        FUN_8005f060(0);
        DAT_1f8001ce = 0;
        FUN_8004fb68(2);
        FUN_8004fa80(2, 1);
        DAT_1f8001d4->w4a++;
        break;
    case 4:
        if (DAT_1f8001ce != 0) {
            g = DAT_1f8001d4;
            g->w48 = 4;
            g->w4a = 0;
        }
        break;
    }
}
