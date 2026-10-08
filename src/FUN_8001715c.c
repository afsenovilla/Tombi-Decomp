// FUNC 8001715c 92 MAIN0
// MATCHING 8001715c 92
extern void FUN_8001747c(void *);
extern void FUN_800171b8(int, int);
void FUN_8001715c(int i)
{
    int k = i * 0x70;
    FUN_8001747c((void *)(k + 0x801fd80c));
    FUN_800171b8(i, *(int *)(k + 0x801fd80c));
}
