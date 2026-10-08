// FUNC 80019244 1144 MAIN0
// MATCHING 80019244 1144
typedef struct {
    char pad[0x48];
    unsigned short w48, w4a, w4c;
    char pad2[0x58 - 0x4e];
    short w58, w5a;
} T80019244;

extern T80019244 *D_1F8001D4;
#define P D_1F8001D4
extern unsigned char D_1F8001D1, D_1F8001D0, D_1F8001CE, D_1F8001C4, D_1F8001C5;
extern int D_1F800164;
extern short D_1F8001F4, D_1F8001F6;

extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
extern short D_8009F3DC;
extern unsigned char D_8009D67C;
extern unsigned char D_8009F0E8;
extern char D_1F8001A0[];
extern char D_80010000[];
extern char D_80010008[];
extern char D_800B3E28[];
extern void titleSequenceTask(void);
void FUN_80020eac(void);
void SetDispMask(int);
void FUN_8004fb68(int);
void FUN_8004fa80(int, int);
void *memset(void *, int, int);
void FUN_8001d63c(int);
void FUN_80020f14(int, int, int, char *);
void FUN_80017328(void (*)(void));
void FUN_80016e34(int, int, int, int);
void FUN_80025b98(void);
void FUN_80016ca8(int, int, int);
void FUN_800196bc(int, int);
void ThreadWaitFrames(int n);

void func_80019244(void)
{
    T80019244 *p;

    D_8009E375 = 0;
    D_8009E376 = 0;
    D_8009E377 = 0;
    D_8009F085 = 0;
    D_8009F086 = 0;
    D_8009F087 = 0;
    D_1F8001D1 = 0;
    D_1F8001D0 = 0;
    D_8009F3DC = 0;
    D_8009D67C = 1;
    D_8009F0E8 = 0;
    FUN_80020eac();
    P->w48 = 9;
    P->w4a = 0;
    P->w4c = 0;
    SetDispMask(0);
    for (;;) {
        switch (P->w48) {
        case 0:
            D_1F8001CE = 0;
            FUN_8004fb68(0);
            FUN_8004fb68(1);
            FUN_8004fa80(0, 1);
            D_1F8001C4 = 0;
            D_1F8001C5 = 0;
            memset(D_1F8001A0, 0, 0x24);
            P->w48++;
            break;
        case 1:
            if (D_1F8001CE) {
                FUN_8001d63c(0x15);
                P->w48 = 4;
            }
            break;
        case 2:
            SetDispMask(1);
            P->w58 = 0x78;
            P->w48++;
            break;
        case 3:
            D_1F800164 = (int)(D_800B3E28 + D_1F8001F4 * 0xc000) & 0xffffff;
            p = P;
            if (--p->w58 == -1)
                p->w48++;
            FUN_80020f14(0x50, 0x60, 0, D_80010000);
            FUN_80020f14(0x50, 0x70, 0, D_80010008);
            break;
        case 4:
            P->w48 = 0;
            P->w4a = 1;
            P->w4c = 0;
            FUN_80017328(titleSequenceTask);
            break;
        case 9:
            SetDispMask(0);
            D_1F8001CE = 0;
            FUN_8004fa80(0x5e, 1);
            P->w48 = 10;
            break;
        case 10:
            if (D_1F8001CE) {
                SetDispMask(0);
                FUN_80016e34(0, 0, 0, 0);
                SetDispMask(1);
                P->w58 = 0xf0;
                P->w48 = 0xb;
                P->w4a = 0;
            }
            break;
        case 11:
            D_1F800164 = (int)(D_800B3E28 + D_1F8001F4 * 0xc000) & 0xffffff;
            p = P;
            switch (p->w4a) {
            case 0:
                p->w5a = 1;
                p->w4a++;
                break;
            case 1:
                if (++p->w5a >= 0x80) {
                    p->w58 = 0xb4;
                    p->w4a++;
                }
                break;
            case 2:
                if (--p->w58 == -1) {
                    p->w4a++;
                    FUN_80025b98();
                }
                break;
            case 3:
                if (--p->w5a == 0) {
                    SetDispMask(0);
                    FUN_80016ca8(0, 0, 0);
                    D_1F8001D1 = 0;
                    D_1F8001F6 = 0;
                    P->w48 = 0;
                    P->w4a = 0;
                }
                break;
            }
            if (P->w48 == 0xb)
                FUN_800196bc(*(unsigned char *)&P->w5a, 1);
            break;
        }
        ThreadWaitFrames(1);
    }
}
