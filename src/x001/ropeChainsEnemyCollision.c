// FUNC 801280dc 52 X001
// MATCHING 801280dc 52
extern short FUN_80127700();

void ropeChainsEnemyCollision(void)
{
    if (FUN_80127700() != 0)
        *(short *)0x1f80019e = 0;
}
