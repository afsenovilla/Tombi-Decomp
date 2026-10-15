// VERSION SLPM_80165 slpm80165 80026688 72
#if defined(VERSION_SLPM_80165)
// Demo-only helper: clears D_8009A81C and copies a 10-entry table into D_8009F740.
typedef struct { int a, b; } E;
extern int D_8009A81C;
extern E D_8009F740[], D_8006C914[];
void func_80026688_slpm80165(void)
{
    int i;
    D_8009A81C = 0;
    for (i = 0; i < 10; i++)
        D_8009F740[i] = D_8006C914[i];
}
#endif
