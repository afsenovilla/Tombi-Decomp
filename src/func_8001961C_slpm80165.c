// VERSION SLPM_80165 slpm80165 8001961C 336
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only game task mode 3: display off and text window cleared, then the movie thread func_8001D1D8 (slot 1,
// movie 4 in D_1F8001CD); Start flags a skip (D_1F8001D3) while it plays; at its end back to the front-end task
// func_80018BBC in mode 1.
typedef struct { char p0[0x48]; unsigned short w48, w4a, w4c, w4e; } Task;
extern Task *D_1F8001D4;
extern unsigned char D_1F8001CC, D_1F8001CD, D_1F8001D3, D_1F8001D0;
extern unsigned short D_1F8001FC;
extern char D_800A2A18[];
extern void SetDispMask(int);
extern void func_80018254(char *);
extern void ThreadCreate(int, void (*)(void));
extern void func_8001D1D8(void);
extern void setTask(void (*)(void));
extern void func_80018BBC(void);

void func_8001961C(void)
{
    Task *t = D_1F8001D4;
    register unsigned short s asm("$4") = t->w4a;
    switch (s) {
    case 0:
        SetDispMask(0);
        func_80018254(D_800A2A18);
        D_1F8001D4->w4a++;
        break;
    case 1:
        t->w4a = s + 1;
        D_1F8001CC = 1;
        D_1F8001CD = 4;
        ThreadCreate(1, func_8001D1D8);
        break;
    case 2:
        if (D_1F8001FC & 0x800) {
            if (D_1F8001CC == 0)
                goto next;
            D_1F8001D3 = 1;
        }
        if (D_1F8001CC)
            break;
    next:
        D_1F8001D4->w4a++;
        break;
    case 3:
        t->w48 = 1;
        t->w4a = 0;
        t->w4c = 0;
        t->w4e = 0;
        D_1F8001D0 = 0;
        setTask(func_80018BBC);
        break;
    }
}
#endif
