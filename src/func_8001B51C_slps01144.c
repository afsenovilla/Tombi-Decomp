// near-twin of PAL areaMode4Handler (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 8001B51C 444
#if defined(VERSION_SLPS_01144)
// VERSION SLPS_01144 slps01144 8001B51C 444
#if defined(VERSION_SLPS_01144)
typedef struct G { char pad[0x4c]; unsigned short w4c, w4e; } G;
extern G *TASK_C;
extern int GAME_TOTAL_TIME_PLAYED[];
extern unsigned char DAT_1f8001c2;
extern volatile unsigned short PAD_PRESSED[];
extern volatile unsigned short DAT_8009d674;
extern unsigned short JOYPAD_STATE;
extern unsigned char HUD_STATE;
extern unsigned char DAT_1f8001cf;
extern unsigned char GAME_KEEP_BGM;
extern unsigned char DISPLAY_BUFFER_0_BG_R, DISPLAY_BUFFER_0_BG_G, DISPLAY_BUFFER_0_BG_B;
extern unsigned char DISPLAY_BUFFER_1_BG_R, DISPLAY_BUFFER_1_BG_G, DISPLAY_BUFFER_1_BG_B;
extern void FUN_800175f0(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void initSectionCamera(void);
extern void spawnSectionUnlayeredObjects(void);
extern void initHudState(void);
extern void startAreaBgm(void);
extern void areaMode4Tick(void);
extern unsigned char *allocObjectLayer7(void);

void func_8001B51C(void)
{
    unsigned char *q;
    char pad[8];
    switch (TASK_C->w4e) {
    case 0:
        FUN_800175f0();
        FUN_80017b44();
        FUN_80021858();
        initSectionCamera();
        spawnSectionUnlayeredObjects();
        initHudState();
        HUD_STATE = 0;
        DAT_1f8001cf = 1;
        if (GAME_KEEP_BGM != 1)
            startAreaBgm();
        TASK_C->w4e++;
        q = allocObjectLayer7();
        if (q != 0) {
            q[0] = 1;
            q[2] = 13;
            q[3] = 0;
            *(short *)(q + 0x12) = 0;
            *(short *)(q + 0x16) = 0;
            *(short *)(q + 0x1a) = 0;
        }
        break;
    case 1:
        GAME_TOTAL_TIME_PLAYED[0]++;
        areaMode4Tick();
        if (DAT_1f8001c2 != 0 && (PAD_PRESSED[0] & 0x800) && (PAD_PRESSED[0] & 8))
            TASK_C->w4e = 3;
        break;
    case 2:
    case 3:
        DISPLAY_BUFFER_0_BG_R = 0;
        DISPLAY_BUFFER_0_BG_G = 0;
        DISPLAY_BUFFER_0_BG_B = 0;
        DISPLAY_BUFFER_1_BG_R = 0;
        DISPLAY_BUFFER_1_BG_G = 0;
        DISPLAY_BUFFER_1_BG_B = 0;
        TASK_C->w4c = 8;
        TASK_C->w4e = 0;
        break;
    }
}
#endif
#endif
