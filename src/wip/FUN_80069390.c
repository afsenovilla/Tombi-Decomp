// FUNC 80069390 56 MAIN0
extern void *(*DAT_800981c0)(void);
extern int FUN_8006ab10(void *, int);
int FUN_80069390(int a, int b)
{
    return FUN_8006ab10((*DAT_800981c0)(), b);
}
