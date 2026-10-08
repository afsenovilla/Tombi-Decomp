// FUNC 8003a498 52 MAIN0
// MATCHING 8003a498 52
typedef struct { char pad0[0x8a]; unsigned short w8a; char pad1[0x1190 - 0x8c]; int i; int v; } G;
extern G *D_8009F0F0;
extern unsigned char D_8009CDA4[];

void func_8003A498(void)
{
    G *g = D_8009F0F0;
    D_8009CDA4[g->i] = g->v;
    g->w8a++;
}
