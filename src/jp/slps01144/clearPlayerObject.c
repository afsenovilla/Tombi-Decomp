// JP slps01144 800172D8 44
// Japanese version of PAL main0 80017D04 (clearPlayerObject.c): 376 -> 360.
extern char PLAYER[];
extern void memset();

void clearPlayerObject(void)
{
    memset(PLAYER, 0, 0x168);
}
