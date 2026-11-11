// VERSION BETA_980203 beta980203 800287E0 140
#if defined(VERSION_BETA_980203)
// Beta version: PAL tickHud.c: the counter branch runs in area 6 sections 0-1 only (PAL the whole area/section word == 6).
extern struct { unsigned char on; char p[7]; short cnt; } DAT_800b1410;
extern unsigned short GAME[];
extern void updateHud(void *);
void tickHud(void)
{
    void *p = &DAT_800b1410;
    if (*(unsigned char *)p != 0) {
        if (GAME[0] == 6 && GAME[1] < 2) {
            if (DAT_800b1410.cnt != 0)
                DAT_800b1410.cnt = DAT_800b1410.cnt - 1;
        } else {
            updateHud(p);
        }
    }
}
#endif
