// FUNC 80017d04 44 MAIN0
// MATCHING 80017d04 44
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x178);
}
