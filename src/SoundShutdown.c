// FUNC 8001f820 48 MAIN0
extern char DAT_80077788;
extern void f1(void);
extern void f2(void);

void SoundShutdown(void)
{
    DAT_80077788 = 0;
    f1();
    f2();
}
