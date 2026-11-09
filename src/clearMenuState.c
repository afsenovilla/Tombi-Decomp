// FUNC 80018254 44 MAIN0
// MATCHING 80018254 44
// VERSION PCPX_96084 tomba 8001A2C4 44
// ASPSX.PCPX_96084 2.34
extern char HUD_STATE[];
extern void *memset(void *, int, int);

void clearMenuState(void)
{
    memset(HUD_STATE, 0, 0x68);
}
