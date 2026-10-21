// FUNC 800ea8f4 936 D0SPSUB
// MATCHING 800ea8f4 936
typedef struct { char p[0x10]; short w10; char p12[2]; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot D_80108848[];
extern Text D_8011113C;
extern int D_80012210;
extern unsigned short D_8009D2A4;
extern unsigned char D_8010A0C8[];
extern unsigned char D_8009CEF7, D_8009CFF8, D_8009CEF8, D_8009C990, D_8009D006, D_8009CFFB;
extern unsigned char D_8009D090[]; /* GAME+0x730 */
#define G30 (D_8009D090 - 0x700) /* GAME+0x30 (same base register in the game) */
extern void subScreenDrawItemIcon(int n, int sel, int flag, int nocheck);

void subScreenDrawItemGrid(Menu *m)
{
    Slot *s;
    for (s = D_80108848; s->x != -1; s++) {
        short i;
        int flag, sel;
        D_8011113C.x = s->x;
        D_8011113C.y = s->y;
        D_8011113C.img = D_80012210;
        D_8011113C.w = m->w10 + 2;
        i = s->idx + m->page * 10;
        if (i >= D_8009D2A4) continue;
        sel = i == m->sel;
        flag = 0;
        switch (D_8010A0C8[i]) {
        case 10:
            if (D_8009CEF7 == 1) flag = 1;
            break;
        case 12:
            if (D_8009CFF8 == 1) flag = 1;
            break;
        case 20:
            if (D_8009CEF8 == 1) flag = 1;
            break;
        case 24:
            if (D_8009D090[0] == 0xFF && D_8009D090[0x223] == 2) flag = 1;
            break;
        case 28:
            if (D_8009D090[0] == 0xFF && D_8009D090[0x223] == 3) flag = 1;
            break;
        case 29:
            if (D_8009D090[0] == 0xFF && D_8009D090[0x223] == 1) flag = 1;
            break;
        case 32:
            if (D_8009D090[0] == 0xFF && D_8009D090[0x223] == 0) flag = 1;
            break;
        case 98:
            if (D_8009C990 == 1) flag = 1;
            break;
        case 99:
            if (D_8009C990 == 2) flag = 1;
            break;
        case 100:
            if (D_8009C990 == 3) flag = 1;
            break;
        case 96:
            if (D_8009D006 != 0) flag = 1;
            break;
        case 132:
            if (D_8009CFFB != 0) flag = 1;
            break;
        case 27:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 0) flag = 1;
            break;
        case 33:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 8) flag = 1;
            break;
        case 34:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 9) flag = 1;
            break;
        case 21:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 5) flag = 1;
            break;
        case 22:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 6) flag = 1;
            break;
        case 23:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 7) flag = 1;
            break;
        }
        subScreenDrawItemIcon(D_8010A0C8[i], sel, flag, m->cursor);
    }
}
