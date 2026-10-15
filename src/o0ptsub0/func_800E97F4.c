// FUNC 800e97f4 104 O0PTSUB0
// MATCHING 800e97f4 104
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char D_800EA634[];
extern char D_800EB124[];
extern int D_800EB070[];
extern void func_800E99FC(MenuItem *m, int a, int b, int c);

void func_800E97F4(short x, short y, int i)
{
    MenuItem m;
    m.x = x;
    m.y = y;
    m.n = 2;
    m.a = D_800EB070[i];
    m.b = D_800EA634;
    m.c = D_800EB124;
    func_800E99FC(&m, 0, 1, 2);
}
