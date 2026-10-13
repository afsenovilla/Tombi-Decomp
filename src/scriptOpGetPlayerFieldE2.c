// FUNC 8003aeb8 36 MAIN0
// MATCHING 8003aeb8 36
extern char *SCRIPT_CTX;
extern unsigned char D_800A611A;

void scriptOpGetPlayerFieldE2(void)
{
    char *p = SCRIPT_CTX;
    *(int *)(p + 0x1190) = D_800A611A;
    *(unsigned short *)(p + 0x8a) += 1;
}
