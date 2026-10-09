// FUNC 80042d5c 288 MAIN0
// MATCHING 80042d5c 288
// Portado de psx_tomba (main/game/actor1.c, resolveHitResultF); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

void computeRotatedQuad(s16* out, s32 arg1, s16 r1, s16 r2);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_8004FE24);

void decreaseObjectTimer(u8* self, u8 arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_800505E8);

s32 resolveHitResultA(u8* arg0, u8* arg1);

s32 resolveHitResultB(u8* arg0, u8* arg1);

s32 resolveHitResultC(u8* arg0, u8* arg1);

s32 resolveHitResultD(u8* arg0, u8* arg1);

s32 resolveHitResultE(u8* arg0, u8* arg1);

s32 resolveHitResultF(u8* arg0, u8* arg1) {
    s32 ret;

    ret = 1;
    arg1[0x68] = 1;
    arg1[0x9E] = 0;
    switch (func_800505E8(arg0, arg1)) {
    case 1:
    case 4:
        playSFX(7);
        if ((*(s16*)(arg1 + 0x98)) != 0) {
            (*(s16*)(arg1 + 0x98)) -= 1;
        }
        func_800224FC(0, *(s16*)(arg0 + 0x12), *(s16*)(arg0 + 0x16), *(s16*)(arg0 + 0x1A));
        break;
    case 2:
    case 8:
        ret = 0;
        arg1[0x9E] = 1;
        arg1[0x9F] = 0;
        arg1[0x68] = 0;
        func_800224FC(1, *(s16*)(arg0 + 0x12), *(s16*)(arg0 + 0x16), *(s16*)(arg0 + 0x1A));
        playSFX(6);
        break;
    case 3:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        playSFX(7);
        (*(s16*)(arg1 + 0x98)) = 0;
        func_800224FC(0, *(s16*)(arg0 + 0x12), *(s16*)(arg0 + 0x16), *(s16*)(arg0 + 0x1A));
        break;
    case 5:
        playSFX(7);
        (*(s16*)(arg1 + 0x98)) -= 2;
        if ((*(s16*)(arg1 + 0x98)) < 0) {
            (*(s16*)(arg1 + 0x98)) = 0;
        }
        func_800224FC(0, *(s16*)(arg0 + 0x12), *(s16*)(arg0 + 0x16), *(s16*)(arg0 + 0x1A));
        break;
    case 0:
        (*(s16*)(arg1 + 0x98)) = 0;
        func_800224FC(0, *(s16*)(arg0 + 0x12), *(s16*)(arg0 + 0x16), *(s16*)(arg0 + 0x1A));
        break;
    }
    return ret;
}

s32 resolveHitResultG(u8* arg0, u8* arg1);

s32 boxesOverlap(u8* arg0, u8* arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051090);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051284);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051488);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051604);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051804);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051A18);

s32 pushOutOfBoxX(u8* arg0, u8* arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051DA4);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80051EE0);

static inline u8 boxesOverlapInline(u8* p0, u8* p1) {
    u8* a = p0;
    u8* b = p1;
    s32 d;
    u16 dx;
    u16 w;

    if ((u16)((*(u16**)(a + 0x44))[1] - (*(u16**)(b + 0x44))[1] + 0x2D) >= 0x5B) {
        return 0;
    }
    dx = (*(u16**)(a + 0x40))[1] - (*(u16**)(b + 0x40))[1];
    w = *(u16*)(a + 0x6C) + *(u16*)(b + 0x6C);
    d = (u16)(dx + w);
    if (*(s16*)(a + 0x6E) + *(s16*)(b + 0x6E) < d) {
        return 0;
    }
    dx = *(u16*)(a + 0x16) - *(u16*)(b + 0x16);
    w = *(u16*)(a + 0x70) + *(u16*)(b + 0x70);
    d = (u16)(dx + w);
    if (*(s16*)(a + 0x72) + *(s16*)(b + 0x72) < d) {
        return 0;
    }
    return 1;
}

void onOverlapSetReaction4(u8* arg0, u8* arg1);

void onOverlapSetReaction3(u8* arg0, u8* arg1);

void func_800522B4(void);

void tryAttachObjectOnResult3(u8* arg0, u8* arg1);

void onOverlapConsumeObject(u8* arg0, u8* arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_8005242C);

s32 boxesOverlapSide(u8* arg0, u8* arg1);

s32 boxesOverlapSigned(u8* arg0, u8* arg1);

s32 boxesOverlapFlipAware(u8* arg0, u8* arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_800527C8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_800529A8);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80052B88);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80052D5C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80052F20);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_800530F0);

void dispatchAreaActorInit(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_8005334C);

void dispatchAreaActorUpdate(void);

void tryAttachObjectOnOverlap(u8* arg0, u8* arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_8005368C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80053808);

void dispatchAreaActorDraw1(void);

void dispatchAreaActorDraw2(void);

void dispatchAreaActorSpawn(void);

void dispatchAreaNpcInit(void);

void applyObjectPush(u8* arg0, u8* arg1);

void callObjectInteraction(void);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80053BB4);

void dispatchObjectContact(u8* arg0, u8* arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80053DA0);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80053F08);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80054618);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80054D60);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_8005548C);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80055A44);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/actor1", func_80055BA0);
