// FUNC 8001b7cc 484 MAIN0
// MATCHING 8001b7cc 484
typedef struct Th { char pad[0x4c]; short b; unsigned short c; } Th;
extern Th *TASK_C;
extern char DAT_1f8001c2, DAT_1f8001cf;
extern unsigned short JOYPAD_STATE;
extern int GAME_TOTAL_TIME_PLAYED[];
extern unsigned char GAME_KEEP_BGM;
extern volatile unsigned short PAD_PRESSED[], DAT_8009d674;
extern char DAT_800b1410;
extern char DISPLAY_BUFFER_0_BG_R, DISPLAY_BUFFER_0_BG_G, DISPLAY_BUFFER_0_BG_B, DISPLAY_BUFFER_1_BG_R, DISPLAY_BUFFER_1_BG_G, DISPLAY_BUFFER_1_BG_B;
extern void InitSubsystems(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void initSectionCamera(void);
extern void spawnSectionUnlayeredObjects(void);
extern void initHudState(void);
extern void startAreaBgm(void);
extern void areaMode5Tick(void);
extern unsigned char *allocObjectLayer7(void);

void areaMode5Handler(void)
{
    Th *t = TASK_C;
    Th *u;
    unsigned char *e;
    char pad[4];
    switch (t->c) {
    case 0:
        InitSubsystems();
        FUN_80017b44();
        FUN_80021858();
        initSectionCamera();
        spawnSectionUnlayeredObjects();
        initHudState();
        DAT_800b1410 = 0;
        DAT_1f8001cf = 1;
        if (GAME_KEEP_BGM != 1) startAreaBgm();
        u = TASK_C;
        u->c = u->c + 1;
        e = allocObjectLayer7();
        if (e) {
            e[0] = 1;
            e[2] = 10;
            e[3] = 0;
            *(short *)(e + 0x12) = 0;
            *(short *)(e + 0x16) = 0;
            *(short *)(e + 0x1a) = 0;
        }
        DAT_8009d674 = 0;
        JOYPAD_STATE = 0;
        PAD_PRESSED[0] = DAT_8009d674;
        break;
    case 1:
        GAME_TOTAL_TIME_PLAYED[0] = GAME_TOTAL_TIME_PLAYED[0] + 1;
        areaMode5Tick();
        if (DAT_1f8001c2 != 0 && (PAD_PRESSED[0] & 8) != 0 && (PAD_PRESSED[0] & 0x800) != 0)
            TASK_C->c = 3;
        break;
    case 2:
    case 3:
        DISPLAY_BUFFER_0_BG_R = 0;
        DISPLAY_BUFFER_0_BG_G = 0;
        DISPLAY_BUFFER_0_BG_B = 0;
        DISPLAY_BUFFER_1_BG_R = 0;
        DISPLAY_BUFFER_1_BG_G = 0;
        DISPLAY_BUFFER_1_BG_B = 0;
        TASK_C->b = 8;
        TASK_C->c = 0;
        break;
    }
}
