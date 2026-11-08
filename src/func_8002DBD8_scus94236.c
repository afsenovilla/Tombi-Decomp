// VERSION SCUS_94236 scus94236 8002DBD8 748
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/game/inventory2.c, func_8002DBD8); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
// INCLUDE_ASM("nonmatchings/main/game/inventory2", func_8002DA2C);
extern s32 WMD_DATA;
extern void (*D_8007C640[])(char *, s32);
// INCLUDE_ASM("nonmatchings/main/game/inventory2", func_8002DBD8);
typedef struct {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 visible;
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 subtype;
    /* 0x04 */ u8 state;
    /* 0x05 */ u8 subState;
    /* 0x06 */ u8 pad06[4];
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 pad0B[5];
    /* 0x10 */ Fix16 x;
    /* 0x14 */ Fix16 y;
    /* 0x18 */ Fix16 z;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ s16 timer;
    /* 0x22 */ u8 pad22[0xE];
    /* 0x30 */ Fix16 posX;
    /* 0x34 */ Fix16 posY;
    /* 0x38 */ Fix16 posZ;
    /* 0x3C */ u8 pad3C[0x30];
    /* 0x6C */ u16 uv[4];
    /* 0x74 */ u8 pad74[0xA];
    /* 0x7E */ s16 velY;
    /* 0x80 */ s16 velX;
    /* 0x82 */ s16 velZ;
    /* 0x84 */ s32 rotX;
    /* 0x88 */ s32 rotY;
    /* 0x8C */ s32 rotZ;
    /* 0x90 */ u8 pad90[0x14];
    /* 0xA4 */ u8 unkA4;
    /* 0xA5 */ u8 padA5[0xF];
    /* 0xB4 */ s16 phaseA;
    /* 0xB6 */ s16 stepA;
    /* 0xB8 */ s16 phaseB;
    /* 0xBA */ s16 stepB;
    /* 0xBC */ u16 spriteId;
    /* 0xBE */ u16 slot;
    /* 0xC0 */ u16 spriteId2;
    /* 0xC2 */ s16 slot2;
    /* 0xC4 */ s16 destX;
    /* 0xC6 */ s16 destY;
    /* 0xC8 */ s16 homeX;
    /* 0xCA */ s16 homeY;
    /* 0xCC */ u16 unkCC;
    /* 0xCE */ u16 cmd;
    /* 0xD0 */ u16 flagD0;
    /* 0xD2 */ u16 eventId;
}
 PopupIcon;
void func_8002DBD8_scus94236(PopupIcon *o)
{
    s32 state = o->state;
    s32 u0, v0, u1, v1;
    s16 s;

    switch (state) {
    case 0:
        o->unkA = 0x14;
        o->unkA4 = 1;
        o->slot2 = acquireSpriteSlot(o->spriteId2);
        o->slot = s = acquireSpriteSlot(o->spriteId);
        u0 = SPRITE_SLOTS[s].val[0] * 4;
        v0 = SPRITE_SLOTS[s].val[1] << 8;
        u1 = u0 + SPRITE_SLOTS[s].val[2];
        v1 = v0 + (SPRITE_SLOTS[s].val[3] << 8);
        o->uv[1] = u0 | v0;
        o->uv[3] = v0 | u1;
        o->uv[0] = u0 | v1;
        o->uv[2] = u1 | v1;
        o->state++;
        break;
    case 1:
        if (D_8009C10A != state) {
            func_8002DEC4(o);
        }
        o->visible = 1;
        pushModelDrawList(o);
        break;
    case 2:
        if (--SPRITE_SLOTS[o->slot].refCount == 0) {
            SPRITE_SLOTS[o->slot].id = -1;
        }
        if (D_8009C618[0] != 3) {
            if (o->cmd & 0x7FFF) {
                if (o->eventId != 0) {
                    setEventStarted(o->eventId, 0, 3);
                }
                if ((o->cmd & 0x7FFF) == state) break;
                if (o->flagD0 == 0 || (o->cmd & 0x7FFF) == 3) {
                    D_8009BCA7_U8Arr[0] = 0;
                    D_8009BCAA_U8Arr[0] = 0;
                    if ((o->cmd & 0x7FFF) != 4 && (D_800A5436[0] == 0 || D_800A5444[0] < 2)) {
                        D_800A539C = 1;
                        D_800A539D = 0;
                        D_800A539E = 0;
                        D_800A539F = 0;
                    } else {
                        D_8009BCA6 = 0;
                    }
                }
            }
            if ((o->cmd & 0x8000) && (o->cmd & 0x7FFF) != 2) {
                D_8009BC9B = 0;
            }
        }
        o->state++;
        break;
    case 3:
        freeModelObject(o);
        break;
    }
}
#endif
