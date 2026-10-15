// FUNC 8001f5b8 104 MAIN0
// MATCHING 8001f5b8 104
// VERSION SLPM_80165 slpm80165 8001EBB4 96
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8001F5B8 (setReverbMode.c): reverb switched on last, no 80076dcc call.
typedef struct S { int a; int b; short c; short d; int e[2]; } S;
extern void FUN_80076dcc(int);
extern void FUN_80076404(S *);
extern void FUN_80076228(int);
extern void FUN_80076e1c(S *);
extern void FUN_80076e94(int, int);
void setReverbMode(unsigned int a, short b)
{
    S s;
    s.a = 7;
    s.b = a | 0x100;
    s.c = b;
    s.d = b;
    FUN_80076404(&s);
    FUN_80076e1c(&s);
    FUN_80076e94(1, 0xffff);
    FUN_80076e94(0, 0xff0000);
    FUN_80076228(1);
}
#else
typedef struct S { int a; int b; short c; short d; int e[2]; } S;
extern void FUN_80076dcc(int);
extern void FUN_80076404(S *);
extern void FUN_80076228(int);
extern void FUN_80076e1c(S *);
extern void FUN_80076e94(int, int);
void setReverbMode(unsigned int a, short b)
{
    S s;
    s.a = 7;
    s.b = a | 0x100;
    s.c = b;
    s.d = b;
    FUN_80076dcc(1);
    FUN_80076404(&s);
    FUN_80076228(1);
    FUN_80076e1c(&s);
    FUN_80076e94(1, 0xffff);
    FUN_80076e94(0, 0xff0000);
}
#endif
