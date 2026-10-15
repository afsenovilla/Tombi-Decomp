// FUNC 800e98b4 328 O0PTSUB0
// MATCHING 800e98b4 328
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char D_800EA634[];
extern int D_800EB0F4[];
extern int D_800EB11C, D_800EB120;
extern void func_800E99FC(MenuItem *m, int a, int b, int c);

void func_800E98B4(short x, short y, int n)
{
    MenuItem m;
    m.n = 9;
    m.x = x;
    m.y = y;
    m.b = D_800EA634;
    if (n > 0) {
        m.a = D_800EB11C;
        func_800E99FC(&m, 0, 1, 0);
    } else if (n < 0) {
        n = -n;
        m.a = D_800EB120;
        func_800E99FC(&m, 0, 1, 0);
    }
    m.x += 8;
    if (n >= 10) {
        m.a = D_800EB0F4[n / 10];
        m.b = D_800EA634;
        func_800E99FC(&m, 0, 1, 0);
        n %= 10;
        m.x += 8;
    }
    m.a = D_800EB0F4[n];
    m.b = D_800EA634;
    func_800E99FC(&m, 0, 1, 0);
}
