// FUNC 8003d9d0 436 MAIN0
// MATCHING 8003d9d0 436
// Ported from psx_tomba (reward.c, rewardJewel); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"

extern int D_80077274;
extern int D_8013A44C;
extern int D_80134018;
extern u8 D_8009C263;
extern u8 D_8009C616;
extern int D_80131D84[];
extern int D_8007728C;

typedef struct {
    int x;
    int y;
    int z;
} VEC3;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    s16 unk6;
    int* unk8;
} unk_8007D6E0;

typedef struct {
    int unk0;
    s16 unk4;
    s16 unk6;
} unk_8007E868;

//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/reward", initItemObject);

void rewardJewel(GameObject* arg0)
{
    int var_a0;
    int var_a2;
    u_char current_item;
    u_char var_v0;
    
    current_item = arg0->item_id;
    switch (current_item) {
        case ITEM_JEWELOFFIRE: 
            if (GAME.redExpLevel == 9) {
                setEventComplete(EVENT_REDHIDDENPOWERS, 1);
                addItemToInventory(arg0->item_id, 1, true);
                asm("");
                if (!(arg0->unkC & 0x80)) {
                    setAreaFlagBit(arg0->objectIndex);
                    asm("");
                }
                arg0->state++;
            } else {
                if ((short)arg0->cooldownTimer == 0) {
                    printInfoMessage(MSG_ITS_LOCKED, MSG_TYPE_INFO);
                    arg0->cooldownTimer = 0x78;
                } 
                arg0->state--;
            }
            break;
        case ITEM_JEWELOFWATER: 
            if (GAME.blueExpLevel == 9) {
                setEventComplete(EVENT_BLUEHIDDENPOWERS, 0);
                addItemToInventory(arg0->item_id, 1, true);
                asm("");
                if (!(arg0->unkC & 0x80)) {
                    setAreaFlagBit(arg0->objectIndex);
                    asm("");
                }
                arg0->state++;
            } else {
                setEventStarted(EVENT_BLUEHIDDENPOWERS, 0, 0);
                if ((short)arg0->cooldownTimer == 0) {
                    printInfoMessage(MSG_ITS_LOCKED, MSG_TYPE_INFO);
                    arg0->cooldownTimer = 0x78;
                } 
                arg0->state--;
            }
            break;
        case ITEM_JEWELOFWIND:
            if (GAME.greenExpLevel == 9) {
                setEventComplete(EVENT_GREENHIDDENPOWERS, 1);
                addItemToInventory(arg0->item_id, 1, true);
                if (!(arg0->unkC & 0x80)) {
                    setAreaFlagBit(arg0->objectIndex);
                }
                arg0->state++;
            } else {
                setEventStarted(EVENT_GREENHIDDENPOWERS, 0, 1);
                if ((short)arg0->cooldownTimer == 0) {
                    printInfoMessage(MSG_ITS_LOCKED, MSG_TYPE_INFO);
                    arg0->cooldownTimer = 0x78;
                } 
                arg0->state--;
            }
            break;
    }
    return;
}
