// FUNC 8004cfd8 564 MAIN0
// MATCHING 8004cfd8 564
#include "TOBJ.H"
typedef struct {
    unsigned short c960; unsigned short c962; char p964[0x15 - 4];
    unsigned char c975; unsigned char c976; char p977[0x963 - 0x17]; unsigned char d2c3;
} G;
extern G GAME;
extern unsigned char *D_8007C178[];
extern unsigned char *D_8007C1A8;
extern unsigned char D_1F8003D1;
extern unsigned char DISPLAY_BUFFER_0_BG_R, DISPLAY_BUFFER_0_BG_G, DISPLAY_BUFFER_0_BG_B;
extern unsigned char DISPLAY_BUFFER_1_BG_R, DISPLAY_BUFFER_1_BG_G, DISPLAY_BUFFER_1_BG_B;
void updateSectionFadeController(TObj *o)
{
    unsigned char *p;
    int r, g, b;
    switch (o->b04) {
    case 0:
        GAME.c975 = 1;
        GAME.c976 = 0;
        p = D_8007C178[GAME.c960];
        if (GAME.c960 == 4 && (GAME.d2c3 & 8)) p = D_8007C1A8;
        p += GAME.c962 * 4;
        r = *p++;
        g = *p;
        b = p[1];
        if (GAME.c960 == 3 && (GAME.d2c3 & 2) && GAME.c962 == GAME.c960) {
            r = 0x60;
            g = 0x97;
            b = 0xff;
        }
        DISPLAY_BUFFER_0_BG_R = r;
        DISPLAY_BUFFER_0_BG_G = g;
        DISPLAY_BUFFER_0_BG_B = b;
        DISPLAY_BUFFER_1_BG_R = r;
        DISPLAY_BUFFER_1_BG_G = g;
        DISPLAY_BUFFER_1_BG_B = b;
        o->b04++;
        break;
    case 1:
        GAME.c975 = 2;
        GAME.c976 += 8;
        if (GAME.c976 != 0) break;
        if (D_1F8003D1 == 1) D_1F8003D1 = 0;
        GAME.c975 = 0;
        o->b04++;
        break;
    case 2:
        if (GAME.c975 == 3) o->b04++;
        break;
    case 3:
        GAME.c975 = 2;
        GAME.c976 -= 8;
        if (GAME.c976 != 0) break;
        GAME.c975 = 1;
        o->b04++;
        DISPLAY_BUFFER_0_BG_R = 0;
        DISPLAY_BUFFER_0_BG_G = 0;
        DISPLAY_BUFFER_0_BG_B = 0;
        DISPLAY_BUFFER_1_BG_R = 0;
        DISPLAY_BUFFER_1_BG_G = 0;
        DISPLAY_BUFFER_1_BG_B = 0;
        break;
    case 4:
        if (GAME.c975 == 4) o->b04 = 0;
        break;
    }
}
