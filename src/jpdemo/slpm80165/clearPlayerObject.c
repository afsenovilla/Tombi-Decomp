// JPDEMO slpm80165 8001698C 44
// Demo version of PAL main0 80017D04 (clearPlayerObject.c): PLAYER is 0x164 bytes (PAL 0x178).
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x164);
}
