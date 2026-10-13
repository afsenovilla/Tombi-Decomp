// FUNC 8003a5a0 52 MAIN0
// MATCHING 8003a5a0 52
typedef struct { char p[0x8a]; unsigned short n; char q[0x1190-0x8c]; int i; int v; } G;
extern G *SCRIPT_CTX;
extern unsigned char SCRIPT_GLOBALS[];
void scriptOpWriteGlobal(void)
{
    G *g = SCRIPT_CTX;
    SCRIPT_GLOBALS[g->i] = g->v;
    g->n++;
}
