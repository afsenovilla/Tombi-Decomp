// FUNC 8003ac50 68 MAIN0
// MATCHING 8003ac50 68
extern char *SCRIPT_CTX;
extern void printInfoMessage(int, int);

void scriptOpShowMessage2(void)
{
    char *p = SCRIPT_CTX;
    printInfoMessage(*(int *)(p + 0x1190), 2);
    *(unsigned short *)(p + 0x8a) += 1;
}
