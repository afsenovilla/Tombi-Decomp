// FUNC 8001ab18 452 MAIN0
// MATCHING 8001ab18 452
typedef struct Th { char pad[0x4c]; short b; unsigned short c; } Th;
extern Th *TASK_C;
extern char DAT_1f8001c2, DAT_1f8001cf;
extern unsigned short JOYPAD_STATE;
extern int GAME_TOTAL_TIME_PLAYED[];
extern unsigned char GAME_KEEP_BGM;
extern int GAME;
extern volatile unsigned short PAD_PRESSED[], DAT_8009d674;
extern char DAT_800b1410;
extern char DAT_8009e375, DAT_8009e376, DAT_8009e377, DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern void InitSubsystems(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void initSectionCamera(void);
extern void spawnSectionUnlayeredObjects(void);
extern void initHudState(void);
extern void startAreaBgm(void);
extern void FUN_8001acdc(void);

void areaMode2Handler(void)
{
    Th *t = TASK_C;
    volatile Th *u;
    char pad[4];
    switch (t->c) {
    case 0:
        InitSubsystems();
        FUN_80017b44();
        DAT_1f8001cf = 1;
        FUN_80021858();
        initSectionCamera();
        spawnSectionUnlayeredObjects();
        initHudState();
        DAT_800b1410 = 2;
        if (GAME_KEEP_BGM != 1 || GAME == 0x30000)
            startAreaBgm();
        u = TASK_C;
        JOYPAD_STATE = 0;
        u->c = u->c + 1;
        PAD_PRESSED[0] = DAT_8009d674 = 0;
        break;
    case 1:
        GAME_TOTAL_TIME_PLAYED[0] = GAME_TOTAL_TIME_PLAYED[0] + 1;
        FUN_8001acdc();
        if (DAT_1f8001c2 != 0 && (PAD_PRESSED[0] & 8) != 0 && (PAD_PRESSED[0] & 0x800) != 0)
            TASK_C->c = 2;
        break;
    case 2:
        DAT_8009e375 = 0;
        DAT_8009e376 = 0;
        DAT_8009e377 = 0;
        DAT_8009f085 = 0;
        DAT_8009f086 = 0;
        DAT_8009f087 = 0;
        t->b = 8;
        t->c = 0;
        break;
    }
}
