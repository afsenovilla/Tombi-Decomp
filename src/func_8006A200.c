// FUNC 8006a200 40 MAIN0
// MATCHING 8006a200 40
extern char *D_80098210;

void func_8006A200(void)
{
    char *p = D_80098210;
    __asm__ volatile ("nop");
    while ((*(volatile unsigned short *)(p + 4) & 2) == 0) {
    }
}
