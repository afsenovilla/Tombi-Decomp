// FUNC 8003d7b8 404 MAIN0
// MATCHING 8003d7b8 404
// Ported from psx_tomba (reward.c, rewardPigBag); MIT licence of the original project.
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

void rewardPigBag(GameObject* arg0)
{
    int temp;

    switch (arg0->subState) {                              // switch 3; irregular
        case 0:                                     // switch 3
            addItemToInventory(arg0->item_id, 1, true);
            if (!(arg0->unkC & 0x80)) {
                setAreaFlagBit(arg0->objectIndex);
            }
            switch (arg0->item_id) {
                case ITEM_REDPIGBAG:
                    setEventComplete(EVENT_ASTORMYPIGBAG, 2);
                    break;
                case ITEM_ORANGEPIGBAG:
                    setEventComplete(EVENT_THEMOUSEPIGBAG, 2);
                    break;
                case ITEM_YELLOWPIGBAG:
                    setEventComplete(EVENT_THEUNDERWATERPIG, 2);
                    break;
                case ITEM_GREENPIGBAG:
                    setEventComplete(EVENT_THEFIREPIGBAG, 2);
                    break;
                case ITEM_BLUEEVILPIGBAG:
                    setEventComplete(EVENT_THEEVILPIGBAG, 2);
                    break;
                case ITEM_NAVYPIGBAG:
                    setEventComplete(EVENT_THEJUNGLEPIGBAG, 2);
                    break;
                case ITEM_PINKPIGBAG:
                    setEventComplete(EVENT_THEHAUNTEDPIGBAG, 2);
                    break;
            }
            arg0->cooldownTimer = 0x168U;
            arg0->subState++;
            return;
        case 1:
            temp = arg0->cooldownTimer - 1;
            arg0->cooldownTimer = temp;
            if ((temp << 0x10) == 0) {
                switch (arg0->item_id) {
                    case ITEM_REDPIGBAG:        
                        setEventStarted(EVENT_PHOENIXMOUNTAIN, 0, 3);
                        break;
                    case ITEM_ORANGEPIGBAG:
                        setEventStarted(EVENT_BACCUSVILLAGE, 0, 3);
                        break;
                    case ITEM_YELLOWPIGBAG:
                        setEventStarted(EVENT_TRICKVILLAGE, 0, 3);
                        break;
                    case ITEM_GREENPIGBAG:
                        setEventStarted(EVENT_LAVACAVES, 0, 3);
                        break;
                    case ITEM_BLUEEVILPIGBAG:
                        setEventStarted(EVENT_THE100FLOWERFOREST, 0, 3);
                        break;
                    case ITEM_NAVYPIGBAG:
                        setEventStarted(EVENT_THEDEEPJUNGLEPIG, 0, 3);
                        break;
                    case ITEM_PINKPIGBAG:
                        setEventStarted(EVENT_THEHAUNTEDMANSION, 0, 3);
                        break;
                }
                arg0->state++;
            }
            return;
    }
}
