// JPDEMO slpm80165 8001FEDC 88
// Demo-only helper: queues load list i and starts task 2 if it is idle.
extern int D_8007005C[];
extern unsigned short D_801FD8E0;
extern unsigned char D_1F8001CE;
extern void func_8001F968(int);
extern void func_8001EE10(void);
extern void ThreadCreate(int, void (*)(void));
void func_8001FEDC(int i)
{
    func_8001F968(D_8007005C[i]);
    D_1F8001CE = 0;
    if (D_801FD8E0 == 0)
        ThreadCreate(2, func_8001EE10);
}
