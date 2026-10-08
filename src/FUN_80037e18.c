// FUNC 80037e18 92 MAIN0
// MATCHING 80037e18 92
extern void FUN_8003bafc(int, int);
extern void FUN_800174fc(int, int, int, int, int);
void FUN_80037e18(int i)
{
int base = *(int *)0x1f800354;
FUN_8003bafc(base + *(int *)(base + (i << 2)), 0x801fbe00);
FUN_800174fc(0x801fbe00, 0x20, 0, 0x80, 0x1ef);
}
