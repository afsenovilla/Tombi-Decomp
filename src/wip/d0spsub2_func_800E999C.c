// FUNC 800e999c 1852 D0SPSUB2
// wip: score 182 (was 359). Memory card save: builds the 0x2000 header (SAVE_DATA_BUFFER) and copies the game block.
// Fixed: the play-time digits with the memCardDrawPlayTime idiom (hh = C968/180000 unsigned, h1 = hh % 1000 unsigned
// then signed digit math, m = C968/3000 - hh*60; block-scoped h100/h10/m10/d4/d1/d2 with `h100 %= 10`), the
// 2-digit values as `q10 = v / 10 (unsigned char); dg[1] = v % 10; dg[0] = q10 % 10`, and block-scoped loop
// variables (`int i; for (i...) ... dg[i]`, index form) which put i in t1 like the game.
// Left (registers only, structure matches): (1) s0/s1 swapped: the game ranks h (SAVE_DATA_BUFFER pointer) above the
// `slot` parameter in global alloc (slot: 4 refs/57 insns beats h: 11 refs/476 here; `unsigned slot` fixes the
// s-regs but the game's slot/10 is signed; `h += 0x80` before the B384 copy also fixes them (h -> s0) but then the
// copy loop's dest is a3: in the game the dest pseudo of the 0x180 block_move_loop is s0 = h, i.e. it got h's
// register by preference, so h must be allocated before that pseudo); (2) in the digit loops the game has
// table -> t0, i -> t1, q -> t2 (ours q -> t0, table -> t2; 6-digit loop: game i t1/q t2, ours t2/t3 because
// our hh%1000 temp takes t1 where the game has t3). Tried: *q++ forms with q/i in either order (198), pointer
// dest (222), SJ **t table variable (210), reusing hh for %1000 (244/182), no h variable at all (369).
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
extern unsigned short BUTTON_CONFIG_INDEX, D_8009D612;
extern unsigned char D_8009D67C, D_8009F0E8;
extern unsigned int GAME_TOTAL_TIME_PLAYED;
extern unsigned char D_8009C980, D_8009C973;
extern B64 *D_800F6A34[];
extern B32 *D_800F6A2C[];
extern B384 *D_800F6A24[];
extern SJ *D_800F66B4[];
extern short D_1F8003D8;
extern void *memset(void *p, int c, int n);
extern void memCardStoreChecksum(void *p, int n);

void func_800E999C(int slot)
{
    char *h = SAVE_DATA_BUFFER;
    unsigned char dg[5];
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
    D_8009C992[0] = D_8009D612;
    *(B64 *)(h + 4) = *D_800F6A34[D_1F8003D8];
    {
        int i;
        int s10 = slot / 10;
        dg[1] = slot % 10;
        dg[0] = s10 % 10;
        for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x16 + i * 2] = *D_800F66B4[dg[i]];
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
        int i; int k = 0;
        for (i = 0; i < 6; i++) {
            if (i != 3) *(SJ *)&h[0x1E + i * 2] = *D_800F66B4[dg[k++]];
        }
    }
    {
        int i;
        unsigned char q10 = D_8009C980 / 10;
        dg[1] = D_8009C980 % 10;
        dg[0] = q10 % 10;
        for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x30 + i * 2] = *D_800F66B4[dg[i]];
        }
    }
    {
        int i;
        unsigned char q10 = D_8009C973 / 10;
        dg[1] = D_8009C973 % 10;
        dg[0] = q10 % 10;
        for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x40 + i * 2] = *D_800F66B4[dg[i]];
        }
    }
    memset(h + 0x44, 0, 0x1C);
    *(B32 *)(h + 0x60) = *D_800F6A2C[D_1F8003D8];
    *(B384 *)(h + 0x80) = *D_800F6A24[D_1F8003D8];
    *(BGame *)D_800E2028 = *(BGame *)GAME;
    memCardStoreChecksum(D_800E2028, 0xA00);
}
