// FUNC 800261b0 140 MAIN0
typedef struct { char c[6]; } S6;
extern S6 DAT_80010238;
extern char DAT_a, DAT_b;
extern void FUN_80069410(int, char *, int);
extern int FUN_80069050();
extern void FUN_80069390(int, S6 *);

void FUN_800261b0(void)
{
    S6 buf = DAT_80010238;
    S6 *q;
    int r;
    DAT_a = 0;
    DAT_b = 0;
    FUN_80069410(0, &DAT_a, 2);
    r = FUN_80069050(0);
    q = &buf;
    if (r == 6)
        FUN_80069390(0, q);
}
