// FUNC 8011bbfc 56 X006
// MATCHING 8011bbfc 56
extern unsigned char HUD_STATE;
extern unsigned char D_800B1412;
extern unsigned char D_800B1413;
extern short D_800B145C;
extern short D_800B145A;

void hudResetState(void)
{
    HUD_STATE = 1;
    D_800B1412 = 0;
    D_800B1413 = 0;
    D_800B145C = 1;
    D_800B145A = 0;
}
