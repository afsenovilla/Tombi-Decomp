// FUNC 8003ab34 68 MAIN0
// MATCHING 8003ab34 68
typedef struct G { char p[0x8a]; unsigned short n; char q[0x1190 - 0x8c]; int a; } G;
extern G *SCRIPT_CTX;
extern void func_8004D620(int, int);
void scriptOpShowMessage(void)
{
    G *g = SCRIPT_CTX;
    func_8004D620(g->a, 2);
    g->n++;
}
