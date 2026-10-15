// FUNC 80114a08 64 X001
// MATCHING 80114a08 64
extern void FUN_801300b0(void);
extern void komaPigGrabbed(void);
void FUN_80114a08(void)
{
    if (*(unsigned short *)0x8009c960 == 3)
        FUN_801300b0();
    else
        komaPigGrabbed();
}
