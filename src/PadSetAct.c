// FUNC 80069410 72 MAIN0
// MATCHING 80069410 72
// CC gcc-2.8.1
extern int (*DAT_800981c0)(void);
extern int _padSetAct(int a, int b, int c);

int PadSetAct(int a, int b, int c)
{
    return _padSetAct(DAT_800981c0(), b, c);
}
