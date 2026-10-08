// FUNC 80124228 20 X004
// MATCHING 80124228 20
void FUN_80124228(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned char *)(*(char **)(p + 0x90) + 0x8c);
}
