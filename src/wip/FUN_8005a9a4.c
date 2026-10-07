// FUNC 8005a9a4 188 MAIN0
extern unsigned char T1[];
extern unsigned char T2[];
extern int T3[];
extern void g1(int a);
extern void g2(int a, int b, int c, int d);
extern void g3(int a, int b);
extern void g4(int a);
extern void g5(void);

int FUN_8005a9a4(int i, int arg)
{
    if (T1[i] != 0xff) {
        T1[i] = 0xff;
        g1(T3[T2[i]]);
        if (i != 10) {
            g2(i, 1, 1, arg);
            g3(i, 1);
            g4(2);
            g5();
        }
    }
    return T1[i];
}
