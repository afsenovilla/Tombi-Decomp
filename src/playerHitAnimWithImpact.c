// FUNC 801229d8 64 X000
// MATCHING 801229d8 64
extern void PlayerSetAnimIfChanged(void *o, int a);
extern void spawnImpactEffect(int a, int b, int c, int d);

void playerHitAnimWithImpact(char *o)
{
    PlayerSetAnimIfChanged(o, 0xd);
    spawnImpactEffect(2, *(short *)(o + 0x12), *(short *)(o + 0x16), *(short *)(o + 0x1a));
}
