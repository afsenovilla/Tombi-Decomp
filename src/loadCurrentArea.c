// FUNC 8001c218 252 MAIN0
// MATCHING 8001c218 252
typedef struct { unsigned short a, b; } P2;
extern P2 GAME;
extern unsigned short AREA_VARIANT;
extern unsigned short DAT_8009c982;
extern unsigned short DAT_8009d2a8, DAT_8009d2aa, DAT_8009d2ac;
extern unsigned short DAT_1f8001de, DAT_1f8001dc;
extern unsigned char DAT_8009c967;
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
    DAT_8009d2a8 = GAME.a;
    DAT_8009d2aa = GAME.b;
    DAT_8009d2ac = DAT_8009c982;
    DAT_1f8001dc = c;
    loadAreaListFile(idx);
    if (loadAreaSectionResources((short)p) != -1)
        DAT_8009c967 = 2;
    queueAreaSectionLists(GAME.a + AREA_VARIANT, GAME.b, (short)(p | u));
    loadAreaSoundBank();
    startSoundTask();
}
