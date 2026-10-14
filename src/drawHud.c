// FUNC 80058680 296 MAIN0
// MATCHING 80058680 296
typedef struct { short x, y; } Pt;
extern void FUN_8005f990(char *);
extern void FUN_80059464(int, int);
extern void drawHudLives(char *, int, int);
extern void drawHudAP(char *, int, int);
extern void drawHudVitality(char *, int, int, int);
extern void drawHudWeaponIcon(char *, int, int);
extern void drawHudItemGauge(char *, int, int, int, int);
extern Pt DAT_800802c8[];
extern short DAT_800802d4[];
extern unsigned char DAT_8009c990;
extern char DAT_800b1468[];

void drawHud(char *o)
{
    char env[0x60];
    int i;
    FUN_8005f990(env);
    FUN_80059464(*(short *)(env + 0x14), 1);
    drawHudLives(o, 0x18, 0xf0);
    drawHudAP(o, 0x12c, 0x10);
    drawHudVitality(o, 0x24, 0x1c, *(short *)(o + 0x52));
    drawHudWeaponIcon(o, 0x48, 0x10);
    for (i = 0; i < 3; i++) {
        if (DAT_8009c990 == DAT_800802d4[i]) {
            drawHudItemGauge(o, i, DAT_800802c8[i].x, DAT_800802c8[i].y, 0);
        } else if (DAT_800b1468[i] != 0) {
            drawHudItemGauge(o, i, DAT_800802c8[i].x, DAT_800802c8[i].y, 1);
        }
    }
}
