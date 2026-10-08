// FUNC 8002e414 160 MAIN0
// MATCHING 8002e414 160
// Ported from psx_tomba (message.c, dispatchAreaDialogInit); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
typedef struct msgBox {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
} msgBox;

























/*
 0x00 = Adquired
 0x01 = Equipped the
 0x02 = Special Message
 0x03 = Special Message Adquired
 0x04 = Hourglass
*/

//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/message", drawInfoMessageText);

void dispatchAreaDialogInit(void)
{
    switch (GAME.selectedArea) {
        case AREA05_BACCUSVILLAGE:
            func_800F07C0();
            return;
        case AREA11_VILLAGEOFCIVILIZATION:
            func_800F6D5C();
            return;
        case AREA16_VILLAGEOFCIVILIZATIONCLOCKTOWER:
            func_800F0C0C();
            return;
        case AREA17_VILLAGEOFCIVILIZATIONIRONTOWER:
            func_800F126C();
            return;
        case AREA08_BACCUSLAKE:
            func_800F1A60();
            return;
        case AREA19_VILLAGEOFCIVILIZATIONPURIFIED:
            func_800F1590();
        default:
            return;
    }
}
