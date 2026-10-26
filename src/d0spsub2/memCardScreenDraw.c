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
extern Card D_800F7200[];
extern short D_800F713C[];
extern short D_800F70FC[][16];
extern Info D_800F74B8[][15];
extern Pos D_800F6A44[];
extern Text D_800F70D8;
extern short D_800F70F4, D_800F70F8;
extern int D_800EDC18;
extern int D_800F714C, D_800F7150, D_800F7154, D_800F7158;
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
    D_800F70D8.w = 1;
    memCardDrawModeTitle(D_800A45F4, 2, 0x10, 0x20);
    m = &MENU_STATE;
    switch (D_800A45D9) {
    case 0:
        break;
    case 1:
        if (D_800F7158 == 0) n = 1;
        else if (D_800F7154 == 0) n = 2;
        else n = 0x11;
        memCardDrawMessage(n);
        memCardDrawSlotSelect(D_800F714C, D_800F7150, D_800F7154);
        break;
    case 2:
    case 3:
    case 4:
        c = &D_800F7200[m->card];
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
            if (D_800F713C[m->card]) {
                D_800F70D8.w = 1;
                memCardDrawFileCursor(2, D_800F6A44[m->cursor].x, D_800F70F4 - (short)(D_800F70F8 - 0x58));
            }
            for (k = 0; k < D_800F713C[m->card]; k++) {
                f = &D_800F74B8[m->card][D_800F70FC[m->card][k]];
                if (f->a == -1) {
                    if (m->x1C) {
                        D_800F70D8.x = D_800F6A44[k].x;
                        D_800F70D8.y = D_800F6A44[k].y - (short)(D_800F70F8 - 0x58);
                        D_800F70D8.w = 1;
                        D_800F70D8.img = D_800EDC18;
                        memCardDrawFileIcon(1, 3, f->icon, k == m->cursor);
                    }
                } else {
                    D_800F70D8.x = D_800F6A44[k].x;
                    D_800F70D8.y = D_800F6A44[k].y - (short)(D_800F70F8 - 0x58);
                    D_800F70D8.w = 1;
                    D_800F70D8.img = D_800EDC18;
                    sel = k == m->cursor;
                    memCardDrawFileIcon(0, 3, f->icon, sel);
                    func_800EB948(f->d, D_800F70D8.x + 0xC0, D_800F70D8.y + 3, sel);
                    func_800EBE6C(f->e, D_800F70D8.x + 0x7C, D_800F70D8.y + 3, sel);
                    memCardDrawNumber2(f->c, D_800F70D8.x + 0x38, D_800F70D8.y + 2, 0);
                    memCardDrawNumber2(f->icon, D_800F70D8.x + 0xA, D_800F70D8.y + 6, 1);
                    memCardDrawNumber2(f->f, D_800F70D8.x + 0x68, D_800F70D8.y + 2, 0);
                    memCardDrawFileLocation(f->a, f->b, D_800F70D8.x + 0x20, D_800F70D8.y + 0x14);
                }
            }
            if (D_800F70F8 > 0) memCardDrawScrollArrow(0, 8, 0x48);
            if (D_800F70F8 < (D_800F713C[m->card] - 3) * 40) memCardDrawScrollArrow(1, 8, 0xC8);
            break;
        }
        break;
    }
}
