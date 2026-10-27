// FUNC 80018254 44 MAIN0
// MATCHING 80018254 44
extern char HUD_STATE[];
extern void *memset(void *, int, int);

void clearMenuState(void)
{
    memset(HUD_STATE, 0, 0x68);
}
