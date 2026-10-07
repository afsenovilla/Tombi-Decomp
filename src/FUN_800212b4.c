// FUNC 800212b4 140 MAIN0
extern int GetGraphType(void);
extern void g(int a, int b, int c, int d, int e);

void FUN_800212b4(short p)
{
    int v = 0x24;
    if (GetGraphType() != 1) {
        v = 0x14;
        if (GetGraphType() == 2) v = 0x24;
    }
    g(0x68, 0x60, 1, v, p);
    g(0xa0, 0x60, 2, v, p);
}
