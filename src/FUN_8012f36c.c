// FUNC 8012f36c 20 X000
// MATCHING 8012f36c 20
void FUN_8012f36c(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned char *)(*(char **)(p + 0x90) + 0x8c);
}
