// VERSION BETA_980203 beta980203 8004D1B8 292
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/drawHud.c with the three-argument drawHudVitality (no vitality value passed).
// USA version of PAL main0 80058680 (drawHud.c): PAL C of src/drawHud.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { short x, y; } Pt;
extern void FUN_8005f990(char *);
extern void FUN_80059464(int, int);
extern void drawHudLives(char *, int, int);
extern void drawHudAP(char *, int, int);
extern void drawHudVitality(char *, int, int);
extern void drawHudWeaponIcon(char *, int, int);
extern void drawHudItemGauge(char *, int, int, int, int);
extern Pt HUD_ITEM_GAUGE_POS[];
extern short DAT_800802d4[];
extern unsigned char DAT_8009c990;
extern char HUD_ITEM_GAUGE_TIMERS[];

void drawHud(char *o)
{
    char env[0x60];
    int i;
    FUN_8005f990(env);
    FUN_80059464(*(short *)(env + 0x14), 1);
    drawHudLives(o, 0x18, 0xd0);
    drawHudAP(o, 0x12c, 0x10);
    drawHudVitality(o, 0x24, 0x1c);
    drawHudWeaponIcon(o, 0x48, 0x10);
    for (i = 0; i < 3; i++) {
        if (DAT_8009c990 == DAT_800802d4[i]) {
            drawHudItemGauge(o, i, HUD_ITEM_GAUGE_POS[i].x, HUD_ITEM_GAUGE_POS[i].y, 0);
        } else if (HUD_ITEM_GAUGE_TIMERS[i] != 0) {
            drawHudItemGauge(o, i, HUD_ITEM_GAUGE_POS[i].x, HUD_ITEM_GAUGE_POS[i].y, 1);
        }
    }
}
#endif
