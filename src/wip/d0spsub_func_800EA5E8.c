// FUNC 800ea5e8 780 D0SPSUB
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
extern short D_8011113C, D_80111140;
extern short D_8011113E[];
extern int D_80111144;
extern unsigned char D_8009CDA4[];
extern void func_800EB2F4(int n);

void func_800EA5E8(Menu *m)
{
    unsigned char d[3];
    unsigned char *q;
    int i, k, n, h;
    short x;
    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (i != 0x12 && D_8009CDA4[i] == 0xFF) n++;
    }
    d[0] = n / 100 % 10;
    d[1] = n / 10 % 10;
    d[2] = n % 10;
    for (q = d, k = 0, x = 8; k < 3; k++) {
        D_8011113C = x;
        x += 0xA;
        D_8011113E[0] = 0xC8;
        D_80111140 = m->w10;
        D_8011113E[2] = 0x7C15;
        D_80111144 = m->p5c[*q++];
        func_800EB2F4(1);
    }
    D_8011113C = 0x20;
    D_8011113E[0] = 0xC8;
    D_80111140 = m->w10;
    D_8011113E[2] = 0x7C15;
    D_80111144 = m->p58[0x18];
    func_800EB2F4(1);
    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (i != 0x12 && D_8009CDA4[i] != 0) n++;
    }
    d[0] = n / 100 % 10;
    d[1] = n / 10 % 10;
    d[2] = n % 10;
    for (q = d, k = 0, x = 0x28; k < 3; k++) {
        D_8011113C = x;
        x += 0xA;
        D_8011113E[0] = 0xC8;
        D_80111140 = m->w10;
        D_8011113E[2] = 0x7C15;
        D_80111144 = m->p5c[*q++];
        func_800EB2F4(1);
    }
}
