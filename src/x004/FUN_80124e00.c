// FUNC 80124e00 20 X004
// MATCHING 80124e00 20
void FUN_80124e00(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned char *)(*(char **)(p + 0x90) + 0x8c);
}
