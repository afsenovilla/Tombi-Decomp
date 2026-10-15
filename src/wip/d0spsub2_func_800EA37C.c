// FUNC 800ea37c 1484 D0SPSUB2
typedef struct { unsigned char state, sub, b2, b3; short timer; short x6; char p8[0xE]; short cursor; short card; short x1A; short x1C; } Menu;
typedef struct { char p0[0x140]; short status, prev, p144, type; char p148[0x10]; short x158; char p15a[0]; } Card;
typedef struct { short icon, a, b, c; int d, e; short p10, f; } Info;
typedef struct { short x, y, w, attr; int img, pC, str; void *p14; } Text;
typedef struct { short x, y; } Pos;
extern Menu D_800A45D8;
extern unsigned char D_800A45D9;
extern short D_800A45F4;
extern Card D_800F7200[];
extern short D_800F713C[];
extern short D_800F70FC[][16];
extern Info D_800F74B8[][15];
extern Pos D_800F6A44[];
extern Text D_800F70D8;
extern short D_800F70DC;
extern short D_800F70F4, D_800F70F8;
extern int D_800EDC18;
extern int D_800F714C, D_800F7150, D_800F7154, D_800F7158;
extern void func_800EA160(Menu *m, int a);
extern void func_800EB0EC(int no, int pri, short x, short y);
extern void func_800EB25C(int n);
extern void func_800EA948(int a, int b, int c);
extern void func_800EAF3C(int pri, short x, short y);
extern void func_800EAC90(int big, int pri, int pal, int lit);
extern void func_800EB948(int n, short x, short y, int bright);
extern void func_800EBE6C(int t, short x, short y, int bright);
extern void func_800EC22C(int n, short x, short y, int force);
extern void func_800EC504(int tbl, int idx, short x, short y);
extern void func_800EC5A4(int n, short x, short y);

void func_800EA37C(void)
{
    Menu *m;
    Card *c;
    Info *f;
    int k, n, sel;
    func_800EA160(m, 0);
    D_800F70D8.w = 1;
    func_800EB0EC(D_800A45F4, 2, 0x10, 0x20);
    m = &D_800A45D8;
    switch ((short)D_800A45D9) {
    case 1:
        if (D_800F7158 == 0) n = 1;
        else if (D_800F7154 == 0) n = 2;
        else n = 0x11;
        func_800EB25C(n);
        func_800EA948(D_800F714C, D_800F7150, D_800F7154);
        break;
    case 2:
    case 3:
    case 4:
        c = &D_800F7200[m->card];
        switch (m->x1A) {
        case 0:
            if (m->x1C == 0) func_800EB25C(0);
            else func_800EB25C(0x13);
            break;
        case 1:
            func_800EB25C(1);
            break;
        case 2:
            func_800EB25C(2);
            break;
        case 3:
            if (m->x1C == 0 && c->x158 == 0) func_800EB25C(0xE);
            else func_800EB25C(0xB);
            break;
        case 4:
            func_800EB25C(3);
            break;
        case 5:
            if (m->x1C) func_800EB25C(4);
            else func_800EB25C(0xE);
            break;
        case 6:
            func_800EB25C(5);
            break;
        case 7:
            func_800EB25C(0xD);
            break;
        case 8:
            func_800EB25C(6);
            break;
        case 9:
            func_800EB25C(7);
            break;
        case 10:
            func_800EB25C(8);
            break;
        case 11:
            func_800EB25C(9);
            break;
        case 12:
            func_800EB25C(0xA);
            break;
        case 13:
            func_800EB25C(0xC);
            break;
        case 14:
            func_800EB25C(0x10);
            break;
        case 15:
            if (m->x1C == 0) func_800EB25C(0x15);
            else func_800EB25C(0x14);
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
                func_800EAF3C(2, D_800F6A44[m->cursor].x, D_800F70F4 - (short)(D_800F70F8 - 0x58));
            }
            for (k = 0; k < D_800F713C[m->card]; k++) {
                f = &D_800F74B8[m->card][D_800F70FC[m->card][k]];
                if (f->a == -1) {
                    if (m->x1C) {
                        D_800F70D8.x = D_800F6A44[k].x;
                        D_800F70D8.w = 1;
                        D_800F70D8.img = D_800EDC18;
                        D_800F70D8.y = D_800F6A44[k].y - (short)(D_800F70F8 - 0x58);
                        func_800EAC90(1, 3, f->icon, k == m->cursor);
                    }
                } else {
                    D_800F70D8.x = D_800F6A44[k].x;
                    D_800F70D8.w = 1;
                    D_800F70D8.img = D_800EDC18;
                    D_800F70D8.y = D_800F6A44[k].y - (short)(D_800F70F8 - 0x58);
                    sel = k == m->cursor;
                    func_800EAC90(0, 3, f->icon, sel);
                    func_800EB948(f->d, D_800F70D8.x + 0xC0, D_800F70D8.y + 3, sel);
                    func_800EBE6C(f->e, D_800F70D8.x + 0x7C, D_800F70D8.y + 3, sel);
                    func_800EC22C(f->c, D_800F70D8.x + 0x38, D_800F70D8.y + 2, 0);
                    func_800EC22C(f->icon, D_800F70D8.x + 0xA, D_800F70D8.y + 6, 1);
                    func_800EC22C(f->f, D_800F70D8.x + 0x68, D_800F70D8.y + 2, 0);
                    func_800EC504(f->a, f->b, D_800F70D8.x + 0x20, D_800F70D8.y + 0x14);
                }
            }
            if (D_800F70F8 > 0) func_800EC5A4(0, 8, 0x48);
            if (D_800F70F8 < (D_800F713C[m->card] - 3) * 40) func_800EC5A4(1, 8, 0xC8);
            break;
        }
        break;
    }
}
