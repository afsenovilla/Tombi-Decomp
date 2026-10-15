// FUNC 800f0e0c 276 D0SPSUB
// MATCHING 800f0e0c 276
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern unsigned short D_8009C960, D_8009C962;
extern int D_80108B78[], D_80108BE4[];
extern int D_80108BCC, D_80108BE0;
extern char D_800FFD98[];
extern int D_80111124;
extern Text D_8011113C;
extern void func_800EBCB8(int a, int b);

void func_800F0E0C(Menu *m, int i)
{
    D_8011113C.x = 0x10;
    D_8011113C.y = 0xD8;
    if (D_8009C960 == 0xE) D_8011113C.str = D_80108B78[D_8009C962 + D_8009C960];
    else D_8011113C.str = D_80108BE4[i];
    switch (D_80111124) {
    case 0x26:
    case 0x27:
    case 0x2E:
        D_8011113C.str = D_80108BCC;
        break;
    case 0xC:
        D_8011113C.str = D_80108BE0;
        break;
    }
    D_8011113C.w = m->w10 + 1;
    D_8011113C.attr = 0x7B92;
    D_8011113C.p14 = D_800FFD98;
    func_800EBCB8(5, 0);
}
