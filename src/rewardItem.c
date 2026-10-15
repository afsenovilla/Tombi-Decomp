// FUNC 8003cc8c 392 MAIN0
// MATCHING 8003cc8c 392
// Ported from psx_tomba (reward.c, rewardItem); MIT licence of the original project.
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

void rewardItem(GameObject* arg0)
{
    int var_a0;
    int var_a1;
    int var_a2;
    u_char current_item;

    addItemToInventory(arg0->item_id, 1, true);
    if (!(arg0->unkC & 0x80)) {
        setAreaFlagBit(arg0->objectIndex);
    }
    current_item = arg0->item_id;
    switch (current_item) {
        case ITEM_BOMB:
            setEventComplete(EVENT_INEEDABOMB, 4);
            break;
        case ITEM_PIPE:
            GAME.pipeState = 1;
            break;
        case ITEM_GOLDENFLOWER:
            GAME.goldenFlowerState = 1;
            break;
        case ITEM_TEARJAR:
            setEventComplete(EVENT_INEEDATEARBOTTLE, 1);
            break;
        case ITEM_MIGHTYFISHFOOD:
            setEventComplete(EVENT_WHATSTHEUNDERWATER, 0);
            break;
        case ITEM_WHATTHETHIEFFORGOT:
            setEventStarted(EVENT_WHATTHETHIEFFORGOT, 1, 1);
            break;
        case ITEM_BOSSJEWEL:
            setEventComplete(EVENT_THEBOSSTREASURE, 1);
            addPlayerAP(100000);
            break;
        case ITEM_SEASHELLNECKLACE:
            setEventStarted(EVENT_THEMERMAIDNECKLACE, 0, 0);
            break;
    }
    arg0->state++;
}
