// FUNC 8001c364 720 MAIN0
// MATCHING 8001c364 720
typedef struct { char p0[0x4e]; unsigned short w4e; } PO;
typedef struct {
    unsigned short a, b; char p00[3]; unsigned char b7; char p01[0x21 - 8]; unsigned char b21; unsigned short c22; char p1[0x434 - 0x24]; unsigned short d434, d436; char p2[0x440 - 0x438]; unsigned short d440;
} G;
extern PO *D_1f8001d4;
extern unsigned char D_1f8001ce;
extern G D_8009c960;
extern unsigned short D_8009c960a, D_8009c962, D_8009cd94, D_8009cd96, DAT_8009c982;
extern unsigned char D_8009c967, D_8009c981;
extern int D_8009f7e4;
extern unsigned short DAT_8009f838;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
extern unsigned char *PTR_80077b64[];
extern void FUN_8004fa80(int, int);
extern void FUN_8001f4bc(void);
extern int FUN_8001beec(void);
extern void FUN_8004f3ec(int);
extern int FUN_8001d1bc(int);
extern void FUN_8004f490(int, int, int);
extern void FUN_80039338(void);
extern void FUN_8004f3bc(void);
extern void FUN_8001be1c(void);

void FUN_8001c364(void)
{
    G *g;
    int f;
    int u;
    int idx;
    unsigned char c;
    switch (D_1f8001d4->w4e) {
    case 0:
        FUN_8004fa80(9, 1);
        D_1f8001d4->w4e++;
        break;
    case 1:
        if (D_1f8001ce != 0) D_1f8001d4->w4e++;
        break;
    case 2:
        D_1f8001d4->w4e++;
        D_8009c960.b7 = 1;
        D_8009c960.b21 = 1;
        break;
    case 3:
        g = &D_8009c960;
        f = g->a != D_8009cd94;
        D_1f8001d4->w4e = 4;
        if (f) {
            D_8009c960.b7 = 2;
            FUN_8001f4bc();
            D_1f8001d4->w4e = 5;
            if (D_8009c960.d434 == 0) {
                if ((unsigned short)(D_8009c960.d436 - 1) < 2) D_8009c960.b21 = 1;
            } else if (D_8009c960.d434 == 1) {
                if (D_8009c960.d436 == 1) D_8009c960.b21 = 1;
            } else {
                D_8009c960.b21 = 0;
            }
        } else {
            if (g->a == 2 && D_8009c962 + D_8009cd96 == 1) D_1f8001d4->w4e = 5;
        }
        g->a = g->d434;
        g->b = g->d436;
        g->c22 = g->d440;
        u = FUN_8001beec() & 0xff;
        idx = D_8009c960a + DAT_8009f838;
        c = PTR_80077b64[idx][D_8009c962];
        DAT_1f8001de = 0;
        DAT_8009d2a8 = D_8009c960a;
        DAT_8009d2aa = D_8009c962;
        DAT_8009d2ac = DAT_8009c982;
        DAT_1f8001dc = c;
        FUN_8004f3ec(idx);
        if (FUN_8001d1bc(f) != -1) D_8009c960.b7 = 2;
        FUN_8004f490(D_8009c960a + DAT_8009f838, D_8009c962, u | f);
        FUN_80039338();
        FUN_8004f3bc();
        D_8009f7e4 = 0;
        break;
    case 4:
        break;
    case 5:
        FUN_8001be1c();
        break;
    }
}
