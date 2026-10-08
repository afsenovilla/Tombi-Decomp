// FUNC 80069410 72 MAIN0
// MATCHING 80069410 72
// CC gcc-2.8.1
extern int (*DAT_800981c0)(void);
extern int FUN_8006a254(int a, int b, int c);

int FUN_80069410(int a, int b, int c)
{
    return FUN_8006a254(DAT_800981c0(), b, c);
}
