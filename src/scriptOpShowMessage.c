// FUNC 8003ab34 68 MAIN0
// MATCHING 8003ab34 68
typedef struct G { char p[0x8a]; unsigned short n; char q[0x1190 - 0x8c]; int a; } G;
extern G *SCRIPT_CTX;
extern void printInfoMessage(int, int);
void scriptOpShowMessage(void)
{
    G *g = SCRIPT_CTX;
    printInfoMessage(g->a, 2);
    g->n++;
}
