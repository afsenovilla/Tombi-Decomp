// FUNC 8003ac50 68 MAIN0
// MATCHING 8003ac50 68
extern char *D_8009F0F0;
extern void func_8004D620(int, int);

void func_8003AC50(void)
{
    char *p = D_8009F0F0;
    func_8004D620(*(int *)(p + 0x1190), 2);
    *(unsigned short *)(p + 0x8a) += 1;
}
