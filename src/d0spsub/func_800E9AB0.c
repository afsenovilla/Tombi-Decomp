// FUNC 800e9ab0 516 D0SPSUB
// MATCHING 800e9ab0 516
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern int D_8009C984;
extern unsigned char D_8009CDAC, D_8009CDB1, D_8009CE3D;
extern char D_800FFD98[];
extern int D_801091A4[];
extern int D_801091A8, D_801091BC, D_801091C0;
extern Text D_8011113C;
extern void func_800EBCB8(int a, int b);

void func_800E9AB0(Menu *m)
{
    int n;
    if (D_8009C984 & 0x40) {
        D_8011113C.x = 0x60;
        D_8011113C.y = 0x98;
        D_8011113C.w = m->w10 + 1;
        D_8011113C.attr = 0x7C15;
        D_8011113C.p14 = D_800FFD98;
        D_8011113C.str = D_801091A8;
        func_800EBCB8(5, 1);
    }
    n = D_8009CDB1;
    if (n >= 2) {
        D_8011113C.x = 0x60;
        D_8011113C.y = 0xA8;
        D_8011113C.w = m->w10 + 1;
        D_8011113C.attr = 0x7C15;
        if (n == 0xFF) n = 5;
        D_8011113C.str = D_801091A4[n];
        D_8011113C.p14 = D_800FFD98;
        func_800EBCB8(5, 1);
    }
    if (D_8009CDAC == 0xFF) {
        D_8011113C.x = 0xF0;
        D_8011113C.y = 0x98;
        D_8011113C.w = m->w10 + 1;
        D_8011113C.attr = 0x7C15;
        D_8011113C.p14 = D_800FFD98;
        D_8011113C.str = D_801091BC;
        func_800EBCB8(5, 1);
    }
    if (D_8009CE3D == 0xFF) {
        D_8011113C.x = 0xF0;
        D_8011113C.y = 0xA8;
        D_8011113C.w = m->w10 + 1;
        D_8011113C.attr = 0x7C15;
        D_8011113C.p14 = D_800FFD98;
        D_8011113C.str = D_801091C0;
        func_800EBCB8(5, 1);
    }
}
