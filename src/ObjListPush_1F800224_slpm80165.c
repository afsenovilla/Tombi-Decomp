// VERSION SLPM_80165 slpm80165 800178EC 60
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80018CF0 (ObjListPush_1F800224.c): list pointer at 0x1F800220, count at 0x1F80024A.
extern int *D_1F800220;
extern unsigned short D_1F80024A;

void ObjListPush_1F800224(int v)
{
    int *p = D_1F800220 - 1;
    D_1F800220 = p;
    *p = v;
    D_1F80024A = D_1F80024A + 1;
}
#endif
