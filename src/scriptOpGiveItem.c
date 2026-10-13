// FUNC 8003acd8 124 MAIN0
// MATCHING 8003acd8 124
extern char *SCRIPT_CTX;
extern void printInfoMessage(int, int);
extern void playSFX(int);
extern void increaseMaxHealth(void);
extern void addItemToInventory(int, int, int);

void scriptOpGiveItem(void)
{
    char *p = SCRIPT_CTX;
    int a = *(int *)(p + 0x1190);
    int b = *(int *)(p + 0x1194);
    int c = *(int *)(p + 0x1198);
    if (a < 0) {
        if (c != 0) {
            printInfoMessage(0x15, 3);
            playSFX(10);
        }
        increaseMaxHealth();
    } else {
        addItemToInventory(a, b, c);
    }
    *(unsigned short *)(p + 0x8a) += 1;
}
