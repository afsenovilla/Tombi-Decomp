// FUNC 8005a8a8 252 MAIN0
// MATCHING 8005a8a8 252
extern unsigned char SCRIPT_FLAGS[];
extern unsigned char DAT_8009cda5;
extern int DAT_8009c960;
extern unsigned char EVENT_STARTED_AP_TABLE[];
extern int AP_TABLE[];
extern void FUN_80026a10(int);
extern void FUN_8005aa74(int, int, int, int);
extern void printEventMessage(int, int);
extern void SfxPlay(int);
extern void spawnItemPickupObject(int);

unsigned char setEventStarted(int p, int b, int c)
{
    if (SCRIPT_FLAGS[p] == 0) {
        if (p == 1) {
            if (DAT_8009c960 == 0) DAT_8009cda5++;
        } else {
            SCRIPT_FLAGS[p]++;
        }
        FUN_80026a10(AP_TABLE[EVENT_STARTED_AP_TABLE[p]]);
        if (p != 10) {
            FUN_8005aa74(p, 0, 0x3c, c);
            printEventMessage(p, 0);
            SfxPlay(0x2a);
            spawnItemPickupObject(0);
        }
    }
    return SCRIPT_FLAGS[p];
}
