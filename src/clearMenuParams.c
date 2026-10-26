// FUNC 80018280 44 MAIN0
// MATCHING 80018280 44
extern char MENU_STATE[];
extern void memset();

void clearMenuParams(void)
{
    memset(MENU_STATE, 0, 0x70);
}
