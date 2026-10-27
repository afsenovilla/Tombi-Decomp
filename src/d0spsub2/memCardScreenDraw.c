// FUNC 800ea37c 1484 D0SPSUB2
// MATCHING 800ea37c 1484
typedef struct { unsigned char state, sub, b2, b3; short timer; short x6; char p8[0xE]; short cursor; short card; short x1A; short x1C; } Menu;
typedef struct { char p0[0x140]; short status, prev, p144, type; char p148[0x10]; short x158; char p15a[0]; } Card;
typedef struct { short icon, a, b, c; int d, e; short p10, f; } Info;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
typedef struct { short x, y; } Pos;
extern Menu MENU_STATE;
extern unsigned char D_800A45D9;
extern short D_800A45F4;
extern Card MEMCARD_CARDS[];
extern short MEMCARD_FILE_COUNT[];
extern short MEMCARD_FILE_ORDER[][16];
extern Info MEMCARD_FILE_INFOS[][15];
extern Pos MEMCARD_FILE_ROW_POSITIONS[];
extern Text MEMCARD_DRAW_PARAMS;
extern short MEMCARD_CURSOR_Y, MEMCARD_LIST_SCROLL;
extern int MEMCARD_FILE_ICON_FRAME;
extern int MEMCARD_SLOT_PRESENT, D_800F7150, MEMCARD_SELECTED_SLOT, MEMCARD_PROBED;
extern void memCardDrawBackground(Menu *m, int a);
extern void memCardDrawModeTitle(int no, int pri, short x, short y);
extern void memCardDrawMessage(int n);
extern void memCardDrawSlotSelect(int a, int b, int c);
extern void memCardDrawFileCursor(int pri, short x, short y);
extern void memCardDrawFileIcon(int big, int pri, int pal, int lit);
extern void func_800EB948(int n, short x, short y, int bright);
extern void func_800EBE6C(int t, short x, short y, int bright);
extern void memCardDrawNumber2(int n, short x, short y, int force);
extern void memCardDrawFileLocation(int tbl, int idx, short x, short y);
extern void memCardDrawScrollArrow(int n, short x, short y);

void memCardScreenDraw(void)
{
    Menu *m;
    Card *c;
    Info *f;
    int k, n, sel;
    memCardDrawBackground(m, 0);
    MEMCARD_DRAW_PARAMS.w = 1;
    memCardDrawModeTitle(D_800A45F4, 2, 0x10, 0x20);
    m = &MENU_STATE;
    switch (D_800A45D9) {
    case 0:
        break;
    case 1:
        if (MEMCARD_PROBED == 0) n = 1;
        else if (MEMCARD_SELECTED_SLOT == 0) n = 2;
        else n = 0x11;
        memCardDrawMessage(n);
        memCardDrawSlotSelect(MEMCARD_SLOT_PRESENT, D_800F7150, MEMCARD_SELECTED_SLOT);
        break;
    case 2:
    case 3:
    case 4:
        c = &MEMCARD_CARDS[m->card];
        switch (m->x1A) {
        case 0:
            if (m->x1C == 0) memCardDrawMessage(0);
            else memCardDrawMessage(0x13);
            break;
        case 1:
            memCardDrawMessage(1);
            break;
        case 2:
            memCardDrawMessage(2);
            break;
        case 3:
            if (m->x1C == 0 && c->x158 == 0) memCardDrawMessage(0xE);
            else memCardDrawMessage(0xB);
            break;
        case 4:
            memCardDrawMessage(3);
            break;
        case 5:
            if (m->x1C) memCardDrawMessage(4);
            else memCardDrawMessage(0xE);
            break;
        case 6:
            memCardDrawMessage(5);
            break;
        case 7:
            memCardDrawMessage(0xD);
            break;
        case 8:
            memCardDrawMessage(6);
            break;
        case 9:
            memCardDrawMessage(7);
            break;
        case 10:
            memCardDrawMessage(8);
            break;
        case 11:
            memCardDrawMessage(9);
            break;
        case 12:
            memCardDrawMessage(0xA);
            break;
        case 13:
            memCardDrawMessage(0xC);
            break;
        case 14:
            memCardDrawMessage(0x10);
            break;
        case 15:
            if (m->x1C == 0) memCardDrawMessage(0x15);
            else memCardDrawMessage(0x14);
            break;
        }
        switch (c->status) {
        case -1:
            break;
        case -2:
        case 1:
        default:
            break;
        case 0:
            if (c->type != 2) break;
            if (MEMCARD_FILE_COUNT[m->card]) {
                MEMCARD_DRAW_PARAMS.w = 1;
                memCardDrawFileCursor(2, MEMCARD_FILE_ROW_POSITIONS[m->cursor].x, MEMCARD_CURSOR_Y - (short)(MEMCARD_LIST_SCROLL - 0x58));
            }
            for (k = 0; k < MEMCARD_FILE_COUNT[m->card]; k++) {
                f = &MEMCARD_FILE_INFOS[m->card][MEMCARD_FILE_ORDER[m->card][k]];
                if (f->a == -1) {
                    if (m->x1C) {
                        MEMCARD_DRAW_PARAMS.x = MEMCARD_FILE_ROW_POSITIONS[k].x;
                        MEMCARD_DRAW_PARAMS.y = MEMCARD_FILE_ROW_POSITIONS[k].y - (short)(MEMCARD_LIST_SCROLL - 0x58);
                        MEMCARD_DRAW_PARAMS.w = 1;
                        MEMCARD_DRAW_PARAMS.img = MEMCARD_FILE_ICON_FRAME;
                        memCardDrawFileIcon(1, 3, f->icon, k == m->cursor);
                    }
                } else {
                    MEMCARD_DRAW_PARAMS.x = MEMCARD_FILE_ROW_POSITIONS[k].x;
                    MEMCARD_DRAW_PARAMS.y = MEMCARD_FILE_ROW_POSITIONS[k].y - (short)(MEMCARD_LIST_SCROLL - 0x58);
                    MEMCARD_DRAW_PARAMS.w = 1;
                    MEMCARD_DRAW_PARAMS.img = MEMCARD_FILE_ICON_FRAME;
                    sel = k == m->cursor;
                    memCardDrawFileIcon(0, 3, f->icon, sel);
                    func_800EB948(f->d, MEMCARD_DRAW_PARAMS.x + 0xC0, MEMCARD_DRAW_PARAMS.y + 3, sel);
                    func_800EBE6C(f->e, MEMCARD_DRAW_PARAMS.x + 0x7C, MEMCARD_DRAW_PARAMS.y + 3, sel);
                    memCardDrawNumber2(f->c, MEMCARD_DRAW_PARAMS.x + 0x38, MEMCARD_DRAW_PARAMS.y + 2, 0);
                    memCardDrawNumber2(f->icon, MEMCARD_DRAW_PARAMS.x + 0xA, MEMCARD_DRAW_PARAMS.y + 6, 1);
                    memCardDrawNumber2(f->f, MEMCARD_DRAW_PARAMS.x + 0x68, MEMCARD_DRAW_PARAMS.y + 2, 0);
                    memCardDrawFileLocation(f->a, f->b, MEMCARD_DRAW_PARAMS.x + 0x20, MEMCARD_DRAW_PARAMS.y + 0x14);
                }
            }
            if (MEMCARD_LIST_SCROLL > 0) memCardDrawScrollArrow(0, 8, 0x48);
            if (MEMCARD_LIST_SCROLL < (MEMCARD_FILE_COUNT[m->card] - 3) * 40) memCardDrawScrollArrow(1, 8, 0xC8);
            break;
        }
        break;
    }
}
