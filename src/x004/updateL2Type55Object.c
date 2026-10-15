// FUNC 80114a08 64 X004
// MATCHING 80114a08 64
extern void FUN_801300b0(void);
extern void updateFreedVillager(void);
void updateL2Type55Object(void)
{
    if (*(unsigned short *)0x8009c960 == 3)
        FUN_801300b0();
    else
        updateFreedVillager();
}
