// FUNC 800f0e0c 276 D0SPSUB
typedef struct { char p[0x10]; short w10; } Menu;
extern unsigned short D_8009C960, D_8009C962;
extern int D_80108B78[], D_80108BE4[];
extern int D_80108BCC, D_80108BE0;
extern char D_800FFD98[];
extern int D_80111124;
extern short D_8011113C, D_8011113E, D_80111140, D_80111142;
extern int D_8011114C;
extern void *D_80111150;
extern void func_800EBCB8(int a, int b);

void func_800F0E0C(Menu *m, int i)
{
    D_8011113C = 0x10;
    D_8011113E = 0xD8;
    if (D_8009C960 == 0xE) D_8011114C = D_80108B78[D_8009C962 + 0xE];
    else D_8011114C = D_80108BE4[i];
    switch (D_80111124) {
    case 0x2E:
        D_8011114C = D_80108BCC;
        break;
    case 0xC:
        D_8011114C = D_80108BE0;
        break;
    }
    D_80111142 = 0x7B92;
    D_80111150 = D_800FFD98;
    D_80111140 = m->w10 + 1;
    func_800EBCB8(5, 0);
}
