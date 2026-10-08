// FUNC 80059728 148 MAIN0
// MATCHING 80059728 148
typedef struct { char p0[3]; char b3, b4, b5, b6, b7; unsigned short s8, sa, sc, se; } P;
extern P *SCR_164;
extern char *SCR_1E0;
extern void g(char *a, P *b);

void FUN_80059728(unsigned short *r, int a, int b, int c)
{
    P *p = SCR_164;
    p->b3 = 3;
    p->b7 = 0x60;
    p->b4 = a;
    p->b5 = b;
    p->b6 = c;
    p->s8 = r[0];
    p->sa = r[1];
    p->sc = r[2];
    p->se = r[3];
    g(SCR_1E0 + 4, p);
    SCR_164 = (P *)((char *)SCR_164 + 0x10);
}
