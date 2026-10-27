// FUNC 80030b9c 268 MAIN0
// MATCHING 80030b9c 268
// Portado de psx_tomba (effect.c, spawnEffectObject); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern u8 D_8009C1C0;

extern u8 D_8009C3F0;
extern u8 D_8009BCA6;

void func_800346A8(u8* self);

void spawnEffectObject(s16 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    ObjectPosView* temp_v0;

    temp_v0 = allocObjectLayer4();
    if (temp_v0 != NULL) {
        temp_v0->unk0 = 1;
        temp_v0->unk2 = 32;
        temp_v0->posX = (s32) (arg1 << 0x10);
        temp_v0->posY = (s32) (arg2 << 0x10);
        temp_v0->posZ = (s32) (arg3 << 0x10);
        temp_v0->unk2E = (s16) (*(u16*)&PLAYER_ANIM_FRAME & 1);
        temp_v0->unk3 = arg0;
        temp_v0->unkD = 0;
        if ((s16)arg0 == 0) {
            temp_v0->unk5 = 1;
        } else {
            temp_v0->unk5 = arg0;
            GAME.unk597 = 1;
        }
        temp_v0->unk6 = 0;
        if (*&GAME.area00_eventControl & 0x100) {
            if ((s16) arg0 == 1) {
                printInfoMessage(MSG_EFFECT_NOTICE, MSG_TYPE_INFO);
            }
            temp_v0->unk4 = 3;
            temp_v0->unk5 = 0;
            temp_v0->unk6 = 0;
        }
    }
}
