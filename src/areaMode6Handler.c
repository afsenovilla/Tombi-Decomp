// FUNC 8001baf4 484 MAIN0
// MATCHING 8001baf4 484
#include "gv6.h"
#include "gamestate.h"
extern unsigned char D_800B1410;
extern volatile unsigned short D_8009D674;

extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
void InitSubsystems(void);
void initHud(void);
void initPlayerAtSpawn(void);
void func_80021B20(void);
void spawnSectionUnlayeredObjects(void);
void initHudState(void);
void startAreaBgm(void);
unsigned char *allocObjectLayer7(void);
void areaMode6Tick(void);
extern unsigned char *TASK_C;
#define G TASK_C
void cutsceneAreaHandler(void)
{
    unsigned char *e;
    volatile unsigned short *k;
    unsigned short t;
    char pad[8];
    switch (*(unsigned short *)(G + 0x4e)) {
    case 0:
        InitSubsystems();
        initHud();
        initPlayerAtSpawn();
        func_80021B20();
        spawnSectionUnlayeredObjects();
        initHudState();
        D_800B1410 = 0;
        *(unsigned char *)0x1F8001CF = 1;
        if (GAME.keepBgm != 1) startAreaBgm();
        (*(unsigned short *)(G + 0x4e))++;
        e = allocObjectLayer7();
        if (e != 0) {
            e[0] = 1;
            e[2] = 0xd;
            e[3] = 0;
            *(short *)(e + 0x12) = 0;
            *(short *)(e + 0x16) = 0;
            *(short *)(e + 0x1a) = 0;
        }
        D_8009D674 = 0;
        t = D_8009D674;
        *(unsigned short *)0x1F8001FC = 0;
        *(volatile unsigned short *)&GV6.s70 = t;
        break;
    case 1:
        ((int*)&GAME.totalTimePlayed)[0]++;
        areaMode6Tick();
        if (*(unsigned char *)0x1F8001C2 != 0) {
            k = &GV6.s70;
            if ((*k & 8) && (*k & 0x800)) *(unsigned short *)(G + 0x4e) = 3;
        }
        break;
    case 2:
    case 3:
        D_8009E375 = 0;
        D_8009E376 = 0;
        D_8009E377 = 0;
        D_8009F085 = 0;
        D_8009F086 = 0;
        D_8009F087 = 0;
        *(unsigned short *)(G + 0x4c) = 8;
        *(unsigned short *)(G + 0x4e) = 0;
        break;
    }
}
