// FUNC 8001715c 92 MAIN0
extern char DAT_801fd80c[];
extern void FUN_8001747c(void *);
extern void FUN_800171b8(int, int);
void FUN_8001715c(int i)
{
    FUN_8001747c(DAT_801fd80c + i * 0x1c * 4);
    FUN_800171b8(i, *(int *)(DAT_801fd80c + i * 0x1c * 4 + 0x...));
}
