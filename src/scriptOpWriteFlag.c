// FUNC 8003a498 52 MAIN0
// MATCHING 8003a498 52
typedef struct { char pad0[0x8a]; unsigned short w8a; char pad1[0x1190 - 0x8c]; int i; int v; } G;
extern G *SCRIPT_CTX;
extern unsigned char SCRIPT_FLAGS[];

void scriptOpWriteFlag(void)
{
    G *g = SCRIPT_CTX;
    SCRIPT_FLAGS[g->i] = g->v;
    g->w8a++;
}
