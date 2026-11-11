// VERSION BETA_980203 beta980203 8001C8A4 172
#if defined(VERSION_BETA_980203)
// Beta version: pending step/state in the scratchpad; no area list file, section resources or sound task here
typedef struct { unsigned short a, b; } P2;
extern P2 GAME;
extern unsigned short AREA_VARIANT;
extern unsigned short GAME_SELECTED_SPAWN_POINT;
extern unsigned short LOADED_AREA, LOADED_SECTION, LOADED_SPAWN_POINT;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
#define PENDING_STEP DAT_1f8001de
#define PENDING_STATE DAT_1f8001dc
extern unsigned char *PTR_80077b64[];
extern int resolveAreaVariant(void);
extern void queueAreaSectionLists(int a, int b, int c);
extern void loadAreaSoundBank(void);

void loadCurrentArea(unsigned short p)
{
    int u;
    int idx;
    unsigned char c;
    u = resolveAreaVariant() & 0xff;
    idx = GAME.a + AREA_VARIANT;
    c = PTR_80077b64[idx][GAME.b];
    PENDING_STEP = 0;
    LOADED_AREA = GAME.a;
    LOADED_SECTION = GAME.b;
    LOADED_SPAWN_POINT = GAME_SELECTED_SPAWN_POINT;
    PENDING_STATE = c;
    queueAreaSectionLists(idx, GAME.b, (short)(p | u));
    loadAreaSoundBank();
}
#endif
