// FUNC 80102a08 84 X002
// MATCHING 80102a08 84
// FLAGS -O2 -G0
extern unsigned short DAT_8009c960;
extern void FUN_80122f54(void);
extern void FUN_8011ccd8(void);

void FUN_80102a08(void)
{
    switch (DAT_8009c960) {
    case 1:
        FUN_80122f54();
        break;
    case 10:
        FUN_8011ccd8();
        break;
    }
}
