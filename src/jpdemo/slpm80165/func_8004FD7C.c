// JPDEMO slpm80165 8004FD7C 92
// Demo-only helper: reloads the system list and switches the main task to mode 1.
extern char D_800A2A18[], D_8006EA60[];
extern int D_80089078;
extern char *D_1F8001D4;
extern void func_80018254(char *);
extern void func_80018650(char *, char *);
void func_8004FD7C(void)
{
    func_80018254(D_800A2A18);
    func_80018650(D_800A2A18, D_8006EA60);
    D_80089078 = 0;
    *(short *)(D_1F8001D4 + 0x48) = 1;
    *(short *)(D_1F8001D4 + 0x4a) = 0;
}
