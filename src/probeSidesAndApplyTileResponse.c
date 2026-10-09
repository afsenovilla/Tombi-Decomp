// FUNC 8003fd78 464 MAIN0
// MATCHING 8003fd78 464
// Portado de psx_tomba (main/game/itemspawn.c, probeSidesAndApplyTileResponse); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

typedef struct { s32 x, y, z; } Vec3L;
s16 probeCollisionAtDepthB(u8*, s16, s16);
extern s16 D_80115320[];
s32 probeCollisionAtDepthA(u8*, s16, s16);
extern s16 D_800A38E8;
extern s16 D_1F80027E;
s16 func_80043D2C(u8*, s16, s16);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/itemspawn", func_8004117C);

void spawnItem(short id, short arg1, int arg2);

void spawnItemDrop(short arg0, short arg1, int arg2);

void spawnItemAtPos(short arg0, short arg1, int arg2, short arg3, short arg4);

void spawnItemDropAtPos(short arg0, short arg1, int arg2, short arg3, short arg4);

void spawnItemBounce(short arg0, short arg1, int arg2, short arg3, short arg4);

void spawnItemFixed(short arg0, short arg1, int arg2);

void spawnItemChest(short arg0, short arg1, int arg2);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/itemspawn", func_80041940);

void dispatchAreaItemInit(void);

void dispatchAreaItemUpdate(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/itemspawn", func_80041DB4);

void spawnItemFromEntry(u8* arg0, s16* arg1, s32 arg2, s32 arg3, u8* arg4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/itemspawn", func_80042204);

void spawnItemLinked(u8* arg0, s16 arg1, s16 arg2, s16* arg3, u16 arg4, u16 arg5);

void dispatchAreaItemDraw(void);

u8* getCollisionColumnPtr(s16 arg0, u8 arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/itemspawn", func_80042654);

inline s32 readCollisionTileShape(u8* arg0);

s32 probeTileShapeThreePoints();

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/itemspawn", func_80042C20);

s32 clampToCeilingAndProbeSides(u8* arg0);

void applyCollisionTileResponse(u8* arg0);

s32 probeSidesAndApplyTileResponse(arg0, arg1, arg2)
u8* arg0;
s16 arg1;
s32 arg2;
{
    u8* obj = arg0;
    s16* src;

    s32 first;
    s32 second;
    s32 ret;

    src = &D_80115320[(*(s16**)(obj + 0x24))[1] * 4];
    *(s16*)(obj + 0x6C) = *src++;
    *(s16*)(obj + 0x6E) = *src++;
    *(s16*)(obj + 0x70) = *src;
    *(s16*)(obj + 0x72) = src[1];
    obj[0x69] = 0;
    *(s16*)(obj + 0xB0) = 0;
    obj[0xA0] = 0;
    obj[0xBE] = 0;
    if (obj[0x9C] != 0) {
        if (*(s16*)(obj + 0x7C) >= 0) {
            first = 8;
            second = -8;
        } else {
            first = -8;
            second = 8;
        }
    } else {
        if (*(s16*)(obj + 0x80) >= 0) {
            first = 8;
            second = -8;
        } else {
            first = -8;
            second = 8;
        }
    }
    ret = probeCollisionAtDepthA(obj, (*(s16**)(obj + 0x40))[1], arg1 + (*(u16*)(obj + 0x16) + *(u16*)(obj + 0x70)));
    if ((s16)ret == 0) {
        ret = probeCollisionAtDepthA(obj, (*(u16**)(obj + 0x40))[1] + first, arg1 + (*(u16*)(obj + 0x16) + *(u16*)(obj + 0x70)));
        if ((s16)ret == 0) {
            ret = probeCollisionAtDepthA(obj, (*(u16**)(obj + 0x40))[1] + second, arg1 + (*(u16*)(obj + 0x16) + *(u16*)(obj + 0x70)));
            if ((s16)ret == 0) {
                return 0;
            }
        }
    }
    if (!arg2) {
        *(u16*)(obj + 0x16) += arg1;
    }
    applyCollisionTileResponse(obj);
    return (s16)ret;
}
