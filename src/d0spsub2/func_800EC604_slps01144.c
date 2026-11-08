// VERSION SLPS_01144 dspsub2 800EC604 1088
#if defined(VERSION_SLPS_01144)
// JP memCardScreenDraw (PAL d0spsub2 800ea37c, USA 800E9EF8): one card, two-column file grid, no slot select
typedef struct { unsigned char state, sub, b2, b3; short timer; short x6; char p8[0xE]; short cursor; short card; short x1A; short x1C; } Menu;
typedef struct { char p0[0x140]; short status, prev, p144, type; char p148[0x10]; short x158; } Card;
typedef struct { short icon, a, b, c; int d, e; short p10, f; } Info;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
typedef struct { short x, y; } Pos;
extern Menu D_800A6120;
extern unsigned char D_800A6121;  /* D_800A6120.sub */
extern short D_800A6138, D_800A613A, D_800A613C; /* D_800A6120.card / x1A / x1C */
extern Card D_800F30D0[];
extern short D_800F30C4;          /* file count */
extern short D_800F30A4[16];      /* file order */
extern Info D_800F3388[15];
extern Pos D_800F2A80[];          /* file row positions */
/* the same table for the file loop: retail's relocations name its y column D_800F2A82 */
extern Pos ROWS_[];
asm(".set ROWS_,D_800F2A82-2");
extern Text D_800F3080;           /* draw params */
extern short D_800F3084;          /* D_800F3080.w */
extern short D_800F309C, D_800F30A0; /* cursor y, list scroll */
extern int D_800EEDF8;            /* file icon frame */
extern void func_800EC3E0(Menu *m, int a); /* memCardDrawBackground */
extern void memCardDrawModeTitle(int no, int pri, short x, short y);
extern void func_800ECFB4(int n); /* memCardDrawMessage */
extern void func_800ECCE8(int pri, int x, int y); /* memCardDrawFileCursor */
extern void func_800ECA44(int big, int pri, int pal, int lit); /* memCardDrawFileIcon */
extern void func_800ED348(int n, int x, int y, int bright); /* memCardDrawSignedNumber */
extern void func_800ED850(int t, int x, int y, int bright); /* memCardDrawPlayTime */
extern void func_800EDD3C(int n, int x, int y); /* memCardDrawNumber2 */
extern void func_800EDFF4(int tbl, int idx, int x, int y); /* memCardDrawFileLocation */
extern void memCardDrawScrollArrow(int n, int x, int y);

void func_800EC604(void)
{
    Menu *m;
    Card *c;
    Info *f;
    int k, sel;
    func_800EC3E0(m, 0);
    D_800F3084 = 1;
    memCardDrawModeTitle(D_800A613C, 2, 0x10, 0x10);
    m = &D_800A6120;
    switch (D_800A6121) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        c = &D_800F30D0[D_800A6138];
        switch (D_800A613A) {
        case 0:
            func_800ECFB4(0);
            break;
        case 1:
            func_800ECFB4(1);
            break;
        case 2:
            func_800ECFB4(2);
            break;
        case 3:
            if (m->x1C == 0 && c->x158 == 0) func_800ECFB4(0xE);
            else func_800ECFB4(0xB);
            break;
        case 4:
            func_800ECFB4(3);
            break;
        case 5:
            func_800ECFB4(4);
            break;
        case 6:
            func_800ECFB4(5);
            break;
        case 7:
            func_800ECFB4(0xD);
            break;
        case 8:
            func_800ECFB4(6);
            break;
        case 9:
            func_800ECFB4(7);
            break;
        case 10:
            func_800ECFB4(8);
            break;
        case 11:
            func_800ECFB4(9);
            break;
        case 12:
            func_800ECFB4(0xA);
            break;
        case 13:
            func_800ECFB4(0xC);
            break;
        case 14:
            func_800ECFB4(0x10);
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
            if (D_800F30C4) {
                D_800F3084 = 1;
                { int s = D_800F30A0 - 0x60; func_800ECCE8(2, D_800F2A80[m->cursor].x, D_800F309C - s); }
            }
            for (k = 0; k < D_800F30C4; k++) {
                f = &D_800F3388[D_800F30A4[k]];
                if (f->a == -1) {
                    if (m->x1C) {
                        D_800F3080.x = ROWS_[k].x;
                        D_800F3080.y = ROWS_[k].y - (short)(D_800F30A0 - 0x60);
                        D_800F3080.w = 1;
                        D_800F3080.img = D_800EEDF8;
                        func_800ECA44(1, 3, f->icon, k == m->cursor);
                    }
                } else {
                    D_800F3080.x = ROWS_[k].x;
                    D_800F3080.y = ROWS_[k].y - (short)(D_800F30A0 - 0x60);
                    D_800F3080.w = 1;
                    D_800F3080.img = D_800EEDF8;
                    sel = k == m->cursor;
                    func_800ECA44(0, 3, f->icon, sel);
                    func_800ED348(f->d, D_800F3080.x + 0x54, D_800F3080.y + 4, sel);
                    func_800ED850(f->e, D_800F3080.x + 0x54, D_800F3080.y + 0xC, sel);
                    func_800EDD3C(f->c, D_800F3080.x + 0x20, D_800F3080.y + 2);
                    func_800EDD3C(f->f, D_800F3080.x + 0x84, D_800F3080.y + 0x16);
                    func_800EDFF4(f->a, f->b, D_800F3080.x + 4, D_800F3080.y + 0x18);
                }
            }
            if (D_800F30A0 > 0) memCardDrawScrollArrow(0, 0x98, 0x51);
            if (D_800F30A0 < (((D_800F30C4 - 1) >> 1) - 1) * 56) memCardDrawScrollArrow(1, 0x98, 0xC6);
            break;
        }
        break;
    }
}
#endif
