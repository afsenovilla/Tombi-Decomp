// FUNC 80114890 64 X002
// MATCHING 80114890 64
// Game has addiu sp first, then lui/lhu; ours schedules lhu before the sp adjust.
extern unsigned int DAT_8009c960;
extern void fa(void);
extern void fb(void);

void FUN_80114890(void)
{
    if ((*(unsigned short *)0x8009c960) == 1) fa();
    else fb();
}
