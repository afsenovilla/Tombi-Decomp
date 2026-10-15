// FUNC 800e9ab0 516 D0SPSUB
typedef struct { char p[0x10]; short w10; } Menu;
extern int D_8009C984;
extern unsigned char D_8009CDAC, D_8009CDB1, D_8009CE3D;
extern char D_800FFD98[];
extern int D_801091A4[];
extern int D_801091A8, D_801091BC, D_801091C0;
typedef struct { short x, y; } Pos;
extern Pos D_8011113C;
extern short D_80111140, D_80111142;
extern int D_8011114C;
extern void *D_80111150;
extern void func_800EBCB8(int a, int b);

static inline void setText2(short w, short attr, void *font, int msg)
{
    D_80111142 = attr;
    D_80111150 = font;
    D_80111140 = w;
    D_8011114C = msg;
}

static inline void setText(short x, short y, short w, short attr, void *font, int msg)
{
    D_8011113C.x = x;
    D_8011113C.y = y;
    setText2(w, attr, font, msg);
}
void func_800E9AB0(Menu *m)
{
    int n;
    if (D_8009C984 & 0x40) {
        D_8011113C.x = 0x60;
        D_8011113C.y = 0x98;
        setText2(m->w10 + 1, 0x7C15, D_800FFD98, D_801091A8);
        func_800EBCB8(5, 1);
    }
    n = D_8009CDB1;
    if (n >= 2) {
        D_8011113C.x = 0x60;
        D_8011113C.y = 0xA8;
        D_80111142 = 0x7C15;
        D_80111140 = m->w10 + 1;
        if (n == 0xFF) n = 5;
        D_80111150 = D_800FFD98;
        D_8011114C = D_801091A4[n];
        func_800EBCB8(5, 1);
    }
    if (D_8009CDAC == 0xFF) {
        D_8011113C.x = 0xF0;
        D_8011113C.y = 0x98;
        D_80111142 = 0x7C15;
        D_80111150 = D_800FFD98;
        D_80111140 = m->w10 + 1;
        D_8011114C = D_801091BC;
        func_800EBCB8(5, 1);
    }
    if (D_8009CE3D == 0xFF) {
        D_8011113C.x = 0xF0;
        D_8011113C.y = 0xA8;
        D_80111142 = 0x7C15;
        D_80111150 = D_800FFD98;
        D_80111140 = m->w10 + 1;
        D_8011114C = D_801091C0;
        func_800EBCB8(5, 1);
    }
}
