// FUNC 80114a08 64 X003
// MATCHING 80114a08 64
extern void updateLostBossItemNpc(void);
extern void FUN_8012d4ac(void);
void updateVillagerNpc(void)
{
    if (*(unsigned short *)0x8009c960 == 3)
        updateLostBossItemNpc();
    else
        FUN_8012d4ac();
}
