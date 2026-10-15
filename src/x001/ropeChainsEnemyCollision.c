// FUNC 801280dc 52 X001
// MATCHING 801280dc 52
extern short landOnSlopedBox();

void ropeChainsEnemyCollision(void)
{
    if (landOnSlopedBox() != 0)
        *(short *)0x1f80019e = 0;
}
