// FUNC 800f0f20 236 D0SPSUB
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { int a, b, c; } Tri;
extern unsigned short D_8009C960, D_8009C962;
extern int D_80108DDC[];
extern int *D_80108F8C[];
extern Tri *D_80108F94;
extern char D_800FFD98[];
extern int D_80111124;
extern short D_8011113C, D_8011113E, D_80111140, D_80111142;
extern int D_8011114C;
extern void *D_80111150;
extern void func_800EBCB8(int a, int b);

void func_800F0F20(Menu *m, int i, int j)
{
    D_8011113C = 0x10;
    D_8011113E = 0xE8;
    if (D_8009C960 == 0xE) D_8011114C = D_80108DDC[D_8009C962];
    else D_8011114C = D_80108F8C[i][j];
    if (D_80111124 == 0xC) D_8011114C = D_80108F94->c;
    D_80111142 = 0x7B92;
    D_80111150 = D_800FFD98;
    D_80111140 = m->w10 + 1;
    func_800EBCB8(5, 0);
}
