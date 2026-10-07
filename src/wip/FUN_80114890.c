// FUNC 80114890 64 X000
extern unsigned short DAT_8009c960;
extern void fa(void);
extern void fb(void);

void FUN_80114890(void)
{
    if (DAT_8009c960 == 1) fa();
    else fb();
}
