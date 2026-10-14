// FUNC 8011fae8 52 X004
// MATCHING 8011fae8 52
extern short pushObjectOutOfObjectX();

void FUN_8011fae8(void)
{
    if (pushObjectOutOfObjectX() != 0)
        *(short *)0x1f80019e = 0;
}
