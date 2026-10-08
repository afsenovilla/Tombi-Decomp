// FUNC 8003ae94 36 MAIN0
// MATCHING 8003ae94 36
typedef struct { char p0[0x8a]; unsigned short c; char p1[0x1190-0x8c]; int v; } G;
extern G *D_8009F0F0;
extern unsigned char D_8009D2B1;

void func_8003AE94(void)
{
    G *g = D_8009F0F0;
    g->v = D_8009D2B1;
    g->c++;
}
