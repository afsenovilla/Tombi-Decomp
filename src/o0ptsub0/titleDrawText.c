// FUNC 800e972c 108 O0PTSUB0
// MATCHING 800e972c 108
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char D_800EA1D8[];
extern int D_800EB040[];
extern void *D_800EB05C[];
extern void titleDrawString(MenuItem *m, int a, int b, int c);

void titleDrawText(short x, short y, int i)
{
    MenuItem m;
    m.n = 6;
    m.x = x;
    m.y = y;
    m.a = D_800EB040[i];
    m.b = D_800EA1D8;
    m.c = D_800EB05C[i];
    titleDrawString(&m, 0, 1, 1);
}
