// JPDEMO x00 8010C3A4 44
// Demo-only helper: runs func_80124AD4 in area 0.
extern unsigned short D_8009A810;
extern void func_80124AD4(void);
void func_8010C3A4(void)
{
    if (D_8009A810 == 0)
        func_80124AD4();
}
