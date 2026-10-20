// FUNC 800ea5e8 780 D0SPSUB
// wip: score 108 (was 175, 776 B): size and the loops now match; left = register allocation only:
// game n=a2 / h=a1 / t=a0 and the 0x12 constant in a1 (ours n=a1, h=a2, t=v1, 0x12 in a2), and the game stores
// d[2] before d[1]. Declaration/init order, loop forms (for/do), unsigned n, hoisted `one` all tried: no effect.
// Keep: digits via quotient reuse (t=n/10, h=n/100), `x += 0xA, k++` in the for header (x in s0, k in s1), the
// straight-line D_8011113C/D_80111140 stores as array[0] (keeps source order), loop `li a0,1` first (f1 form).
typedef struct { char p[0x10]; short w10; char p12[0x46]; int *p58; int *p5c; } Menu;
extern short D_8011113C, D_80111140;
extern short D_8011113E[];
extern int D_80111144;
extern short D_8011113C_a[], D_80111140_a[];
extern unsigned char D_8009CDA4[];
extern void func_800EB2F4(int n);

void func_800EA5E8(Menu *m)
{
    unsigned char d[3];
    unsigned char *q;
    int i, k, n, h, t;
    short x;
    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (i != 0x12 && D_8009CDA4[i] == 0xFF) n++;
    }
    t = n / 10;
    h = n / 100;
    d[0] = h % 10;
    d[1] = t - h * 10;
    d[2] = n - t * 10;
    for (q = d, k = 0, x = 8; k < 3; x += 0xA, k++) {
        D_8011113C = x;
        D_8011113E[0] = 0xC8;
        D_80111140 = m->w10;
        D_8011113E[2] = 0x7C15;
        D_80111144 = m->p5c[*q++];
        func_800EB2F4(1);
    }
    D_8011113C_a[0] = 0x20;
    D_8011113E[0] = 0xC8;
    D_80111140_a[0] = m->w10;
    D_8011113E[2] = 0x7C15;
    D_80111144 = m->p58[0x18];
    func_800EB2F4(1);
    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (i != 0x12 && D_8009CDA4[i] != 0) n++;
    }
    t = n / 10;
    h = n / 100;
    d[0] = h % 10;
    d[1] = t - h * 10;
    d[2] = n - t * 10;
    for (q = d, k = 0, x = 0x28; k < 3; x += 0xA, k++) {
        D_8011113C = x;
        D_8011113E[0] = 0xC8;
        D_80111140 = m->w10;
        D_8011113E[2] = 0x7C15;
        D_80111144 = m->p5c[*q++];
        func_800EB2F4(1);
    }
}
