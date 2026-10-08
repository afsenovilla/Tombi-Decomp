// FUNC 8004e41c 1272 MAIN0
// MATCHING 8004e41c 1272
typedef struct {
    char p0[0x48]; short w48; unsigned short w4a; unsigned short w4c; char p4e[0x58 - 0x4e]; short w58;
    char p5a[0x68 - 0x5a]; unsigned char b68, b69, b6a, b6b;
} G;
extern G *D_1f8001d4;
extern unsigned char D_1f8001cc;
extern short D_1f8001f4;
extern unsigned short D_1f8001fc, D_1f8001f8;
extern int D_1f800164;
extern char D_800B3E28[];
extern unsigned short D_8007DE98[];
extern unsigned short D_8009D670;
extern void func_800198B4();
extern void SetDispMask(int);
extern void FUN_80016ca8(int, int, int);
extern void func_800E8A5C(void);
extern void FUN_8001eff8(int);
extern void func_800E972C(int, int, int);
extern void func_800E8A94(int *);
extern void playSFX(int);
extern void stopBgm(int);
extern void FUN_8001e9e8(int, int);
extern void FUN_80017328(void *);
extern void func_800E9D08(int, int);
extern void updateSound(void);
void func_8004E41C(int *p)
{
    int d;
    switch (D_1f8001d4->w4a) {
    case 0:
        SetDispMask(0);
        D_1f8001d4->w4c = 0;
        D_1f8001d4->w4a++;
    case 1:
        if (D_1f8001cc == 0) {
            FUN_80016ca8(0, 0, 0);
            D_1f8001d4->w4a++;
        }
        break;
    case 2:
        func_800E8A5C();
        SetDispMask(1);
        FUN_8001eff8(0);
        D_1f800164 = (int)(D_800B3E28 + D_1f8001f4 * 0xc000) & 0xffffff;
        func_800E972C(0x30, 0xd0, 1);
        D_1f8001d4->b69 = 0;
        D_1f8001d4->b6a = D_8007DE98[D_1f8001d4->b69];
        D_1f8001d4->b6b = D_8007DE98[D_1f8001d4->b69];
        D_1f8001d4->w58 = 0x3cc;
        D_1f8001d4->w4a++;
        break;
    case 3:
        D_1f800164 = (int)(D_800B3E28 + D_1f8001f4 * 0xc000) & 0xffffff;
        if (--D_1f8001d4->w58 <= 0) {
            D_1f8001d4->w4a++;
        }
        func_800E8A94(p);
        if (*(volatile unsigned short *)&D_8009D670 != 0) {
            D_1f8001d4->w58 = 0x3cc;
        }
        switch (D_1f8001d4->w4c) {
        case 0:
            if ((D_1f8001fc & 0x80) && D_1f8001d4->b69 != 0) {
                D_1f8001d4->b69--;
                D_1f8001d4->b6b = D_8007DE98[D_1f8001d4->b69];
                D_1f8001d4->w4c++;
                playSFX(8);
            }
            if ((D_1f8001fc & 0x20) && D_1f8001d4->b69 < 2) {
                D_1f8001d4->b69++;
                D_1f8001d4->b6b = D_8007DE98[D_1f8001d4->b69];
                D_1f8001d4->w4c++;
                playSFX(8);
            }
            if (D_1f8001fc & 0x4008) {
                stopBgm(0);
                switch (D_1f8001d4->b69) {
                case 0:
                    D_1f8001d4->b68 = 0;
                    FUN_8001e9e8(10, 10);
                    FUN_80017328(func_800198B4);
                    break;
                case 1:
                    D_1f8001d4->b68 = 1;
                    FUN_8001e9e8(10, 10);
                    FUN_80017328(func_800198B4);
                    break;
                case 2:
                    D_1f8001d4->w48 = 2;
                    D_1f8001d4->w4a = 0;
                    FUN_8001e9e8(10, 10);
                    break;
                }
            }
            break;
        case 1:
            if (D_1f8001d4->b6a != D_1f8001d4->b6b) {
                d = -4;
                if (D_1f8001d4->b6a < D_1f8001d4->b6b) d = 4;
                D_1f8001d4->b6a += d;
                if (D_1f8001d4->b6b == D_1f8001d4->b6a) {
                    D_1f8001d4->w4c--;
                }
            }
            break;
        }
        func_800E972C(0x30, 0xd0, 1);
        if (D_1f8001f8 & 0x20) {
            func_800E972C(0x38, 0xa0, 0);
        }
        func_800E9D08(D_1f8001d4->b6a, D_1f8001d4->b69);
        updateSound();
        break;
    case 4:
        stopBgm(0);
        *p = 0;
        D_1f8001d4->w48 = 3;
        D_1f8001d4->w4a = 0;
        break;
    }
}
