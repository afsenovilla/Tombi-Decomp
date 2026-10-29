// FUNC 8001715c 92 MAIN0
// MATCHING 8001715c 92
// VERSION SLPS_01144 slps01144 800166E4 92
extern void setTaskEntry(void *);
extern void FUN_800171b8(int, int);
void startTask(int i)
{
    int k = i * 0x70;
    setTaskEntry((void *)(k + 0x801fd80c));
    FUN_800171b8(i, *(int *)(k + 0x801fd80c));
}
