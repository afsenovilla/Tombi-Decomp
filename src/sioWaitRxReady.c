// FUNC 8006a200 40 MAIN0
// MATCHING 8006a200 40
extern char *PAD_SIO;

void sioWaitRxReady(void)
{
    char *p = PAD_SIO;
    __asm__ volatile ("nop");
    while ((*(volatile unsigned short *)(p + 4) & 2) == 0) {
    }
}
