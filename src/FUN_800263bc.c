// FUNC 800263bc 276 MAIN0
// MATCHING 800263bc 276
typedef struct { char a; char pad[0x1f]; void *p[6]; char pad2[0x5c - 0x38]; char r[3][4]; } GT;
extern GT G;
extern int D1454;
extern char D1412, D1413;
extern short D145c, D145a, D1462, D1460, D1464;
extern int D1428;
extern void *D142c;
extern int S2d8;
extern unsigned char D3690;
extern unsigned char D2f81;
extern char A2048[], A2068[], A20ec[], A2100[], A2150[], A22a0[], A24f8[], A2678[];

void FUN_800263bc(void)
{
    int i;
    GT *g = &G;
    g->a = 1;
    D1454 = -1;
    D1412 = 0;
    D1413 = 0;
    D145c = 600;
    D145a = 0;
    D1428 = S2d8;
    D1462 = D3690;
    D1460 = D3690;
    D1464 = D3690;
    if (D2f81 == 0)
        D142c = A2048;
    else
        D142c = A2068;
    g->p[0] = A20ec;
    g->p[1] = A2100;
    g->p[2] = A2150;
    g->p[3] = A22a0;
    g->p[4] = A24f8;
    g->p[5] = A2678;
    i = 0;
    do {
        g->r[0][i] = 0;
        g->r[1][i] = 0;
        g->r[2][i] = 0;
        i++;
    } while (i < 3);
}
