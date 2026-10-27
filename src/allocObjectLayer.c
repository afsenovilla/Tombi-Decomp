// FUNC 8001832c 140 MAIN0
// MATCHING 8001832c 140
// Ported from psx_tomba (gameinit.c, allocObjectLayer); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/gameinit", allocObjectLayer3);

void* allocObjectLayer(u8 arg0)
{
    s16  n = FREE_OBJECT_COUNT;
    s32* p;
    ObjectAxisView* obj;

    if (n > 0) {
        p = FREE_OBJECTS;
        FREE_OBJECT_COUNT = n - 1;
        FREE_OBJECTS = p + 1;
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
