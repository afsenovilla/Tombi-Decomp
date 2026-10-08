// FUNC 8002dc88 64 MAIN0
// MATCHING 8002dc88 64
// Ported from psx_tomba (message.c, showMessageBoxTimed); MIT licence of the original project.
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

void showMessageBoxTimed(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4)
{
    msgBox box;

    box.unk2 = arg2;
    box.unk6 = arg3;
    box.unkA = 0;
    func_80030800(arg0, arg1, &box, 0, arg4);
}
