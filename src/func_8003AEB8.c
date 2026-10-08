// FUNC 8003aeb8 36 MAIN0
// MATCHING 8003aeb8 36
extern char *D_8009F0F0;
extern unsigned char D_800A611A;

void func_8003AEB8(void)
{
    char *p = D_8009F0F0;
    *(int *)(p + 0x1190) = D_800A611A;
    *(unsigned short *)(p + 0x8a) += 1;
}
