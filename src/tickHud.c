// FUNC 800264d0 116 MAIN0
// MATCHING 800264d0 116
extern struct { unsigned char on; char p[7]; short cnt; } DAT_800b1410;
extern int GAME;
extern void updateHud(void *);
void tickHud(void)
{
    if (DAT_800b1410.on != 0) {
        if (GAME == 6) {
            if (DAT_800b1410.cnt != 0)
                DAT_800b1410.cnt = DAT_800b1410.cnt - 1;
        } else {
            updateHud(&DAT_800b1410);
        }
    }
}
