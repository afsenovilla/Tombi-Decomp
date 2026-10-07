// FUNC 8001db38 164 MAIN0
extern void g1(int a);
extern void g2(void *a);
extern void g3(void *a);
extern void g4(void *a, int b);
extern void g5(int a, int b, int c, int d, int e);
extern int g6(int a, int b, int c);
extern int g7(int a);
extern char F2[], F3[], F4[];

void FUN_8001db38(int a)
{
    g1(0);
    g2(F2);
    g3(F3);
    g4(F4, 0x20);
    g5(0, 1, -1, 0, 0);
    while (!g6(2, a, 0) || !g7(0x1c0)) ;
}
