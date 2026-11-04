// VERSION SLPS_01144 slps01144 80064BEC 140
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern void func_800584CC(void);
extern void func_80064F4C(void);
extern void func_800584DC(void);
extern void func_80059C6C(int);
extern void func_80064D64(void);
extern void func_80064EF4(int, int, int, int);
extern int D_8009BFE4;
void func_80064BEC(int a, int b, int c, int d)
{
    func_800584CC();
    func_80064F4C();
    func_800584DC();
    func_80059C6C(0);
    func_80064D64();
    func_80064EF4(a, b, c, d);
    D_8009BFE4 = 1;
}
#endif
