// FUNC 8003a5a0 52 MAIN0
// MATCHING 8003a5a0 52
typedef struct { char p[0x8a]; unsigned short n; char q[0x1190-0x8c]; int i; int v; } G;
extern G *D_8009F0F0;
extern unsigned char D_8009CEA4[];
void func_8003A5A0(void)
{
    G *g = D_8009F0F0;
    D_8009CEA4[g->i] = g->v;
    g->n++;
}
