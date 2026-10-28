// FUNC 80122b2c 20 X010
// MATCHING 80122b2c 20
void copyParentB8cToD8cX10(char *p)
{
    *(unsigned *)(p + 0x8c) = *(unsigned char *)(*(char **)(p + 0x90) + 0x8c);
}
