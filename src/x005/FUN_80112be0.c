// FUNC 80112be0 68 X005
// MATCHING 80112be0 68
extern unsigned char DAT_8009c964;
extern unsigned char DAT_8009c93a;
extern void fa(void);

void FUN_80112be0(void)
{
    if (DAT_8009c964 != 0x21 || DAT_8009c93a != 1) fa();
}
