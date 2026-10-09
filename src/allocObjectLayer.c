// FUNC 8001832c 140 MAIN0
// MATCHING 8001832c 140
// Ported from psx_tomba (gameinit.c, allocObjectLayer); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);














const char BUILD_DATE[] asm("D_80010000") = "98/3/22";

const char BUILD_TIME[] asm("D_80010008") = "21:11";

void* allocObjectLayer(u8 arg0)
{
    s16  n = D_1F800238;
    s32* p;
    ObjectAxisView* obj;

    if (n > 0) {
        p = D_1F800208;
        D_1F800238 = n - 1;
        D_1F800208 = p + 1;
        obj = (ObjectAxisView*)*p;
        obj->layer = arg0;
        if ((D_1F8001C8 & 1) == 0) {
            obj->drawBufA = &obj->data[0x10];
            obj->drawBufB = &obj->data[0x18];
        } else {
            obj->drawBufB = &obj->data[0x10];
            obj->drawBufA = &obj->data[0x18];
        }
        return obj;
    }
    return NULL;
}
