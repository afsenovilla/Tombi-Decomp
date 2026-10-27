// FUNC 8004fed4 496 MAIN0
// MATCHING 8004fed4 496
// Portado de psx_tomba (actorrender1.c, getObjectRenderKind); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern SVECTOR D_1F800060;
extern long D_1F80008C;

#define gte_rtps_real() __asm__ volatile("nop;" "nop;" ".word 0x4A180001")

int getObjectRenderKind(u8* self)
{
    s16 d;

    switch (GAME.selectedArea) {
    case 0:
        if (GAME_SELECTED_SECTION == 3) {
            return self[0xA];
        }
        break;
    case 2:
        if (GAME_SELECTED_SECTION == 0) {
            break;
        }
        if (GAME_SELECTED_SECTION == 3) {
            break;
        }
        return self[0xA];
    case 6:
        if ((u_int)(GAME_SELECTED_SECTION - 1) >= 2) {
            break;
        }
        return self[0xA];
    case 5:
    case 8:
        if (GAME_SELECTED_SECTION == 1 || GAME_SELECTED_SECTION == 3) {
            return self[0xA];
        }
        break;
    case 9:
        if (GAME_SELECTED_SECTION == 0) {
            break;
        }
        if (GAME_SELECTED_SECTION == 6) {
            break;
        }
        return self[0xA];
    case 10:
        if (GAME_SELECTED_SECTION == 8) {
            return self[0xA];
        }
        break;
    case 13:
    case 19:
        if (GAME_SELECTED_SECTION == 1) {
            return self[0xA];
        }
        break;
    case 18:
        if (GAME_SELECTED_SECTION == 2) {
            return self[0xA];
        }
        break;
    case 11:
    case 16:
    case 17:
        return self[0xA];
    }
    if (self[0xA] < 4) {
        switch (*(u16*)0x1F8001C8 & 1) {
        case 0:
            d = *(u16*)0x1F8000F6 - *(u16*)(self + 0x1A);
            break;
        case 1:
            d = *(u16*)(self + 0x12) - *(u16*)0x1F8000EE;
            break;
        }
        if (d != 0 && !(self[0xA] & 1)) {
            if (GAME.selectedArea == 4) {
                *(int*)0x1F8002B4 = d * 5 + 0x1000;
            } else {
                *(int*)0x1F8002B4 = d * 7 + 0x1000;
            }
            return self[0xA] | 1;
        }
    }
    return self[0xA];
}
