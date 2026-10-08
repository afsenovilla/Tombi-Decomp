// FUNC 80122070 20 X010
// MATCHING 80122070 20
void FUN_80122070(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned char *)(*(char **)(p + 0x90) + 0x8c);
}
