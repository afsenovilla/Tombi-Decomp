// FUNC 8003e918 72 MAIN0
// MATCHING 8003e918 72
extern unsigned short DAT_8009c960;
extern void FUN_80123648(void);
extern void FUN_8011e4ec(void);

void FUN_8003e918(void)
{
    if (DAT_8009c960 == 0) {
        FUN_80123648();
    } else if (DAT_8009c960 == 4) {
        FUN_8011e4ec();
    }
}
