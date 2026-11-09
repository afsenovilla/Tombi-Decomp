// FUNC 800e999c 1852 D0SPSUB2
// MATCHING 800e999c 1852
// Memory card save: builds the 0x2000 header (SAVE_DATA_BUFFER) and copies the game block.
// Matched 2026-11-08. What fixed the last registers:
// (1) the digit loops: ONE function-scope `int i` and `unsigned char *p` shared by all four loops, `p = dg;` before
//     each loop and `*D_800F66B4[*p++]` (the game keeps i in t1 and p in t2 in every loop);
// (2) slot/h in s1/s0: `slot` copied through a barriered `register int t asm("$17")`, so h gets s0;
// (3) the 0x180 block copy running on s0 itself: the two tail copies go through a barriered
//     `register char *g asm("$16") = h` (the copy's dest pseudo then prefers s0), and the B384 index is read through
//     a `const` alias of D_1F8003D8 (`asm("D_1F8003D8")`), so cse keeps the second load and sched hoists it above
//     the B32 copy like the game.
typedef struct { int d[3]; } B12;
typedef struct { char c[2]; } SJ;
typedef struct { char d[64]; } B64;
typedef struct { char d[32]; } B32;
typedef struct { char d[0x180]; } B384;
typedef struct { char d[0x988]; } BGame;
extern char SAVE_DATA_BUFFER[];
extern char D_800E1E29, D_800E1E2A, D_800E1E2B;
extern char D_800E2028[];
extern char GAME[];
extern int D_8009D2B4[], PLAYER_POS[];
extern short GAME_SELECTED_SPAWN_POINT[];
extern char D_8009C972[], D_8009D2E4[], D_8009D2E5[], D_8009D2E6[], D_8009C992[];
extern unsigned short BUTTON_CONFIG_INDEX, PAD_VIBRATION_ENABLED;
extern unsigned char D_8009D67C, D_8009F0E8;
extern unsigned int GAME_TOTAL_TIME_PLAYED;
extern unsigned char D_8009C980, D_8009C973;
extern B64 *D_800F6A34[];
extern B32 *D_800F6A2C[];
extern B384 *D_800F6A24[];
extern SJ *D_800F66B4[];
extern short D_1F8003D8;
extern const short D_1F8003D8c asm("D_1F8003D8");
extern void *memset(void *p, int c, int n);
extern void memCardStoreChecksum(void *p, int n);

void func_800E999C(int slot_)
{
    int slot;
    char *h = SAVE_DATA_BUFFER;
    unsigned char dg[5];
    int i;
    unsigned char *p;
    { register int t asm("$17"); t = slot_; asm("" : "=r"(t) : "0"(t)); slot = t; }
    memset(h, 0, 0x2000);
    *(B12 *)D_8009D2B4 = *(B12 *)PLAYER_POS;
    GAME_SELECTED_SPAWN_POINT[0] = 0;
    D_8009C972[0] = 1;
    h[0] = 'S';
    h[1] = 'C';
    h[2] = 0x11;
    h[3] = 1;
    D_8009D2E6[0] = BUTTON_CONFIG_INDEX;
    D_8009D2E4[0] = D_8009D67C;
    D_8009D2E5[0] = D_8009F0E8;
    D_8009C992[0] = PAD_VIBRATION_ENABLED;
    *(B64 *)(h + 4) = *D_800F6A34[D_1F8003D8];
    {
        int s10 = slot / 10;
        dg[1] = slot % 10;
        dg[0] = s10 % 10;
        p = dg; for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x16 + i * 2] = *D_800F66B4[*p++];
        }
    }
    {
        unsigned int hh = GAME_TOTAL_TIME_PLAYED / 180000;
        int h1 = hh % 1000;
        int m = GAME_TOTAL_TIME_PLAYED / 3000 - hh * 60;
        {
            int m10, h100, h10; unsigned char d4, d1, d2;
            h100 = h1 / 100; h10 = h1 / 10; m10 = m / 10;
            d4 = m - m10 * 10;
            d1 = h10 - h100 * 10;
            d2 = h1 - h10 * 10;
            h100 %= 10;
            dg[0] = h100;
            m10 %= 10;
            dg[3] = m10;
            dg[4] = d4;
            dg[1] = d1;
            dg[2] = d2;
        }
    }
    {
        p = dg; for (i = 0; i < 6; i++) {
            if (i != 3) *(SJ *)&h[0x1E + i * 2] = *D_800F66B4[*p++];
        }
    }
    {
        unsigned char q10 = D_8009C980 / 10;
        dg[1] = D_8009C980 % 10;
        dg[0] = q10 % 10;
        p = dg; for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x30 + i * 2] = *D_800F66B4[*p++];
        }
    }
    {
        unsigned char q10 = D_8009C973 / 10;
        dg[1] = D_8009C973 % 10;
        dg[0] = q10 % 10;
        p = dg; for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x40 + i * 2] = *D_800F66B4[*p++];
        }
    }
    memset(h + 0x44, 0, 0x1C);
    {
        register char *g asm("$16");
        g = h;
        asm("" : "=r"(g) : "0"(g));
        *(B32 *)(g + 0x60) = *D_800F6A2C[D_1F8003D8];
        *(B384 *)(g + 0x80) = *D_800F6A24[D_1F8003D8c];
    }
    *(BGame *)D_800E2028 = *(BGame *)GAME;
    memCardStoreChecksum(D_800E2028, 0xA00);
}
