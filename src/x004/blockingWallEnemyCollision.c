// FUNC 8011fae8 52 X004
// MATCHING 8011fae8 52
extern short pushObjectOutOfObjectX();

void blockingWallEnemyCollision(void)
{
    if (pushObjectOutOfObjectX() != 0)
        *(short *)0x1f80019e = 0;
}
