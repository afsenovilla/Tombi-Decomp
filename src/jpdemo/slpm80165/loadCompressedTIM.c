// JPDEMO slpm80165 80035E74 92
// Demo version of PAL main0 80037E18 (loadCompressedTIM.c): CLUT row 0x1e8 (PAL 0x1ef).
extern void lzDecompress(int, int);
extern void loadTIM(int, int, int, int, int);
void loadCompressedTIM(int i)
{
int base = *(int *)0x1f800354;
lzDecompress(base + *(int *)(base + (i << 2)), 0x801fbe00);
loadTIM(0x801fbe00, 0x20, 0, 0x80, 0x1e8);
}
