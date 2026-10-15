// FUNC 800e985c 88 O0PTSUB0
// MATCHING 800e985c 88
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char D_800EA634[];
extern int D_800EB070[];
extern void func_800E99FC(MenuItem *m, int a, int b, int c);

void func_800E985C(short x, short y, int i, int c)
{
    MenuItem m;
    m.n = 9;
    m.x = x;
    m.y = y;
    m.a = D_800EB070[i];
    m.b = D_800EA634;
    func_800E99FC(&m, 0, 1, c);
}
