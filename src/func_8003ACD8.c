// FUNC 8003acd8 124 MAIN0
// MATCHING 8003acd8 124
extern char *D_8009F0F0;
extern void func_8004D620(int, int);
extern void playSFX(int);
extern void FUN_80026f4c(void);
extern void addItemToInventory(int, int, int);

void func_8003ACD8(void)
{
    char *p = D_8009F0F0;
    int a = *(int *)(p + 0x1190);
    int b = *(int *)(p + 0x1194);
    int c = *(int *)(p + 0x1198);
    if (a < 0) {
        if (c != 0) {
            func_8004D620(0x15, 3);
            playSFX(10);
        }
        FUN_80026f4c();
    } else {
        addItemToInventory(a, b, c);
    }
    *(unsigned short *)(p + 0x8a) += 1;
}
