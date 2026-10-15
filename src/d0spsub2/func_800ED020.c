// FUNC 800ed020 168 D0SPSUB2
// MATCHING 800ed020 168
extern int D_8009F0F4, D_8009F0F8, D_8009F0FC, D_8009F100;
extern int D_8009F10C, D_8009F110, D_8009F114, D_8009F118;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int CloseEvent(int ev);

void func_800ED020(void)
{
    EnterCriticalSection();
    CloseEvent(D_8009F10C);
    CloseEvent(D_8009F110);
    CloseEvent(D_8009F114);
    CloseEvent(D_8009F118);
    CloseEvent(D_8009F0F4);
    CloseEvent(D_8009F0F8);
    CloseEvent(D_8009F0FC);
    CloseEvent(D_8009F100);
    ExitCriticalSection();
}
