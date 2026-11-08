// VERSION SCUS_94236 scus94236 8002DEC4 1252
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/game/inventory2.c, func_8002DEC4); licencia MIT del proyecto original.
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
// INCLUDE_ASM("nonmatchings/main/game/inventory2", func_8002DEC4);
void func_8002DEC4_scus94236(PopupIcon *o)
{
    s32 done;
    s32 u0, v0;
    char first;
    s32 u1, v1;
    s32 dx;
    s32 yy;
    s32 px, py;
    s32 t;

    switch (o->subState) {
    case 0:
        o->rotX = 0;
        o->rotY = 0;
        o->rotZ = 0;
        o->subState++;
        break;
    case 1:
        done = --o->timer == 0;
        if (done) {
            o->velY = -0x800;
            o->subState++;
            o->timer = 30;
            dx = o->x.p.whole - 0xA0;
            px = (o->destX - dx) << 8;
            o->velX = px / 30;
            yy = o->y.p.whole + 0x78;
            dx = yy;
            px = (o->destY - dx) << 8;
            o->velZ = px / 30;
        }
        break;
    case 2:
        o->posX.raw += o->velX << 8;
        o->posY.raw += o->velZ << 8;
        o->posY.raw += o->velY << 8;
        o->velY += 0x88;
        if (--o->timer == 0) {
            first = o->subtype == 0;
            o->timer = first ? 0xB4 : 0x5A;
            o->phaseA = 0;
            o->stepA = (nextRandom() & 3) + 3;
            o->phaseB = 0;
            o->stepB = (nextRandom() & 7) + 3;
            o->subState++;
        }
        break;
    case 3:
        if (o->subtype == 0) {
            o->posY.raw = ((u16)o->destY + (s16)fixedMulSin(o->phaseA, 4)) << 16;
        }
        o->rotZ = (s16)fixedMulSin(o->phaseB, 0x80);
        o->phaseA += o->stepA;
        o->phaseB += o->stepB;
        o->phaseA &= 0xFF;
        o->phaseB &= 0xFF;
        if (--o->timer == 0) {
            o->rotZ = 0;
            if (o->subtype == 0) {
                o->subState++;
            } else {
                o->timer = 1;
                o->subState = 5;
            }
        }
        break;
    case 4:
        o->posX.raw += 0x80000;
        if (o->posX.raw > 0x1400000) {
            o->state++;
        }
        break;
    case 5:
        if (--o->timer == 0) {
            o->timer = 30;
            o->rotX = 0;
            o->subState++;
        }
        break;
    case 6:
        o->posX.raw += (((u16)o->homeX << 16) - o->posX.raw) >> NTSC_PAL(3, 2);
        o->posY.raw += (((u16)o->homeY << 16) - o->posY.raw) >> NTSC_PAL(3, 2);
        o->rotX += 0x40;
        if (o->rotX >= 0x400) {
            if (o->unkCC >= 0x80) {
                u0 = SPRITE_SLOTS[o->slot2].val[0] * 4;
                v0 = SPRITE_SLOTS[o->slot2].val[1] << 8;
                u1 = u0 + SPRITE_SLOTS[o->slot2].val[2];
                v1 = v0 + (SPRITE_SLOTS[o->slot2].val[3] << 8);
                o->rotX = 0xC00;
                o->subState++;
                o->uv[1] = u0 | v0;
                o->uv[3] = v0 | u1;
                o->uv[0] = u0 | v1;
                o->uv[2] = u1 | v1;
            } else {
                o->state++;
            }
        }
        break;
    case 7:
        o->rotX += 0x40;
        if (o->rotX == 0x1000) {
            o->timer = (nextRandom() & 0x1F) + 0x5A;
            o->subState++;
        }
    case 8:
        if (--o->timer == 0) {
            o->stepA = (nextRandom() & 0x7F) - 0x40;
            o->stepB = (nextRandom() & 0x7F) - 0x40;
            o->phaseB = (nextRandom() & 0x7F) - 0x40;
            o->velY = nextRandom() & 0x13;
            o->subState++;
        }
        break;
    case 9:
        o->rotX = (o->rotX + (u16)o->stepA) & 0xFFF;
        o->rotY = (o->rotY + (u16)o->stepB) & 0xFFF;
        o->rotZ = (o->rotZ + (u16)o->phaseB) & 0xFFF;
        o->velY += 2;
        o->posY.raw += o->velY << 12;
        if (o->posY.raw > 0x1100000) {
            o->state++;
        }
        break;
    }
    t = o->posX.p.whole + 0xA0;
    o->x.p.whole = t;
    py = o->posZ.p.whole;
    o->z.p.whole = py;
    t = o->posY.p.whole - 0x78;
    o->y.p.whole = t;
}
#endif
