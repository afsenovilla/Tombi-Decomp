// FUNC 8012f36c 20 X000
// MATCHING 8012f36c 20
void bitingPlantCopyParentAngle(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned char *)(*(char **)(p + 0x90) + 0x8c);
}
