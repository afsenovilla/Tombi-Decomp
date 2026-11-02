// FUNC 800264d0 116 MAIN0
// MATCHING 800264d0 116
// VERSION SLPS_01144 slps01144 800272BC 140
#if defined(VERSION_SLPS_01144)
typedef struct { unsigned char on; char p[7]; short cnt; } Hud;
extern Hud D_800B2EE0;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern void updateHud(Hud *);
void func_800272BC(void)
{
    Hud *h = &D_800B2EE0;
    if (h->on != 0) {
        if (GAME == 6 && GAME_SELECTED_SECTION < 2) {
            if (h->cnt != 0)
                h->cnt = h->cnt - 1;
        } else {
            updateHud(h);
        }
    }
}
#else
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
#endif
