// FUNC 800261b0 140 MAIN0
extern unsigned char DAT_8009d614;
extern unsigned char DAT_8009d615;
extern void FUN_80069410(int, unsigned char *, int);
extern int FUN_80069050(int);
extern void FUN_80069390(int, unsigned char *);

void FUN_800261b0(void)
{
    unsigned char align[6] = {0, 1, 0xff, 0xff, 0xff, 0xff};
    DAT_8009d614 = 0;
    DAT_8009d615 = 0;
    FUN_80069410(0, &DAT_8009d614, 2);
    if (FUN_80069050(0) == 6)
        FUN_80069390(0, align);
}
