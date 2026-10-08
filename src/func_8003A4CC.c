// FUNC 8003a4cc 48 MAIN0
// MATCHING 8003a4cc 48
typedef struct { char pad[0x8a]; unsigned short w8a; char pad2[0x1190 - 0x8c]; int d1190; } G;
extern G *D_8009F0F0;
extern unsigned char D_8009CDA4[];

void func_8003A4CC(void)
{
    G *g = D_8009F0F0;
    int v = D_8009CDA4[g->d1190];
    g->w8a++;
    g->d1190 = v;
}
