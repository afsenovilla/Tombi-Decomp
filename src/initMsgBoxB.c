// FUNC 8002c09c 68 MAIN0
// MATCHING 8002c09c 68
// Ported from psx_tomba (message.c, initMsgBoxB); MIT licence of the original project.
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

void initMsgBoxB(u8* self)
{
    *(void**)(self + 0x24) = &D_80014C8C;
    readAnimFrameCount(self);
    self[4] = 1;
    self[5] = 1;
    self[6] = 0;
}
