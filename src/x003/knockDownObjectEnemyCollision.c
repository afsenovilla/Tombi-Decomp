// FUNC 801203d0 52 X003
// MATCHING 801203d0 52
extern short pushObjectOutOfObjectBoxB();

void knockDownObjectEnemyCollision(void)
{
    if (pushObjectOutOfObjectBoxB() != 0)
        *(short *)0x1f80019e = 0;
}
