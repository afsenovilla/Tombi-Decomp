// FUNC 800f0f20 236 D0SPSUB
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { int a, b, c; } Tri;
extern unsigned short D_8009C960, D_8009C962;
extern int D_80108DDC[];
extern int *D_80108F8C[];
extern Tri *D_80108F94;
extern char D_800FFD98[];
extern int D_80111124;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern Text D_8011113C;
extern void func_800EBCB8(int a, int b);

void func_800F0F20(Menu *m, int i, int j)
{
    D_8011113C.x = 0x10;
    D_8011113C.y = 0xE8;
    if (D_8009C960 == 0xE) D_8011113C.str = D_80108DDC[D_8009C962];
    else D_8011113C.str = D_80108F8C[i][j];
    if (D_80111124 == 0xC) D_8011113C.str = D_80108F94->c;
    D_8011113C.w = m->w10 + 1;
    D_8011113C.attr = 0x7B92;
    D_8011113C.p14 = D_800FFD98;
    func_800EBCB8(5, 0);
}
