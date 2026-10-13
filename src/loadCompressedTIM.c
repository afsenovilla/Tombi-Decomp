// FUNC 80037e18 92 MAIN0
// MATCHING 80037e18 92
extern void lzDecompress(int, int);
extern void loadTIM(int, int, int, int, int);
void loadCompressedTIM(int i)
{
int base = *(int *)0x1f800354;
lzDecompress(base + *(int *)(base + (i << 2)), 0x801fbe00);
loadTIM(0x801fbe00, 0x20, 0, 0x80, 0x1ef);
}
