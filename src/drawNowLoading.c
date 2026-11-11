// FUNC 800212b4 140 MAIN0
// MATCHING 800212b4 140
// VERSION BETA_980203 beta980203 80023750 140
#if defined(VERSION_BETA_980203)
// Beta version of PAL main0 800212B4 (drawNowLoading.c): PAL C of src/drawNowLoading.c, edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
extern int GetGraphType(void);
extern void g(int a, int b, int c, int d, int e);

void drawNowLoading(short p)
{
    int v;
    if (GetGraphType() == 1 || GetGraphType() == 2) v = 0x24;
    else v = 0x14;
    g(0x70, 0x60, 1, v, p);
    g(0x98, 0x60, 2, v, p);
}
#else
extern int GetGraphType(void);
extern void g(int a, int b, int c, int d, int e);

void drawNowLoading(short p)
{
    int v;
    if (GetGraphType() == 1 || GetGraphType() == 2) v = 0x24;
    else v = 0x14;
    g(0x68, 0x60, 1, v, p);
    g(0xa0, 0x60, 2, v, p);
}
#endif
