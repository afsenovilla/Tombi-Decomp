// FUNC 8006a200 40 MAIN0
extern char *PTR_JOY;

void FUN_8006a200(void)
{
    char *p = PTR_JOY;
    do {
    } while ((*(volatile unsigned short *)(p + 4) & 2) == 0);
}
