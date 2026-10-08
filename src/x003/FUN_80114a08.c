// FUNC 80114a08 64 X003
// MATCHING 80114a08 64
extern void FUN_801300b0(void);
extern void FUN_8012d4ac(void);
void FUN_80114a08(void)
{
    if (*(unsigned short *)0x8009c960 == 3)
        FUN_801300b0();
    else
        FUN_8012d4ac();
}
