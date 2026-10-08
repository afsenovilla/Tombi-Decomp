// FUNC 8001f820 48 MAIN0
// MATCHING 8001f820 48
extern char DAT_80078788;
extern void f1(void);
extern void f2(void);

void SoundShutdown(void)
{
    DAT_80078788 = 0;
    f1();
    f2();
}
