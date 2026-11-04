// FUNC 8001c218 252 MAIN0
// MATCHING 8001c218 252
// VERSION SLPS_01144 slps01144 8001BC7C 172
#if defined(VERSION_SLPS_01144)
typedef struct { unsigned short a, b; } P2;
extern P2 GAME;
extern unsigned short AREA_VARIANT;
extern unsigned short D_8009E52A;
extern unsigned short LOADED_AREA, LOADED_SECTION, LOADED_SPAWN_POINT;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
extern unsigned char *D_80071644[];
extern int resolveAreaVariant(void);
extern void func_8001FFFC(int a, int b, int c);
extern void loadAreaSoundBank(void);
void func_8001BC7C(unsigned short p)
{
    int u;
    int idx;
    unsigned char c;
    u = resolveAreaVariant() & 0xff;
    idx = GAME.a + AREA_VARIANT;
    c = D_80071644[idx][GAME.b];
    LOADED_SECTION = GAME.b;
    DAT_1f8001de = 0;
    LOADED_AREA = GAME.a;
    LOADED_SPAWN_POINT = D_8009E52A;
    DAT_1f8001dc = c;
    func_8001FFFC(idx, GAME.b, (short)(p | u));
    loadAreaSoundBank();
}
#else
typedef struct { unsigned short a, b; } P2;
extern P2 GAME;
extern unsigned short AREA_VARIANT;
extern unsigned short GAME_SELECTED_SPAWN_POINT;
extern unsigned short LOADED_AREA, LOADED_SECTION, LOADED_SPAWN_POINT;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
extern unsigned char GAME_KEEP_BGM;
extern unsigned char *PTR_80077b64[];
extern int FUN_8001beec(void);
extern void loadAreaListFile(int);
extern int loadAreaSectionResources(int a);
extern void queueAreaSectionLists(int a, int b, int c);
extern void loadAreaSoundBank(void);
extern void startSoundTask(void);

void loadCurrentArea(unsigned short p)
{
    int u;
    int idx;
    unsigned char c;
    u = FUN_8001beec() & 0xff;
    idx = GAME.a + AREA_VARIANT;
    c = PTR_80077b64[idx][GAME.b];
    DAT_1f8001de = 0;
    LOADED_AREA = GAME.a;
    LOADED_SECTION = GAME.b;
    LOADED_SPAWN_POINT = GAME_SELECTED_SPAWN_POINT;
    DAT_1f8001dc = c;
    loadAreaListFile(idx);
    if (loadAreaSectionResources((short)p) != -1)
        GAME_KEEP_BGM = 2;
    queueAreaSectionLists(GAME.a + AREA_VARIANT, GAME.b, (short)(p | u));
    loadAreaSoundBank();
    startSoundTask();
}
#endif
