// FUNC 8003c550 628 MAIN0
// MATCHING 8003c550 628
// Portado de psx_tomba (main/game/reward.c, updateL1Type02Reward); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

void updateItemPickupAnim(GameObject* arg0);

extern int D_80077274;
extern int D_8013B104;
extern int D_80134D0C;
extern u8 D_8009CEFB;
extern u8 D_8009D2AE;
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

void updateThrownObject(u8* self);

void func_8003F78C(u8* self);

void updateL1Type02Reward(u8* self)
{
    u8* obj;
    int sfx;

    switch (self[4]) {
    case 0:
        self[0] = 2;
        self[0xA] = 2;
        self[0xA5] = 1;
        self[0xD] = 0;
        *(int*)(self + 0x8C) = 0;
        *(s16*)(self + 0x6C) = 10;
        *(s16*)(self + 0x6E) = 0x14;
        *(s16*)(self + 0x70) = 0x10;
        *(s16*)(self + 0x72) = 0x20;
        self[4]++;
        *(int*)(self + 0x3C) = *(int*)0x1F8002D4;
        if (GAME.selectedArea == 0) {
            *(s16*)(self + 0x1E) = 10;
            *(int*)(self + 0x24) = D_8013B104;
        } else {
            *(s16*)(self + 0x1E) = 8;
            *(int*)(self + 0x24) = D_80134D0C;
        }
        readAnimFrameCount(self);
        func_80022E44(self);
        break;
    case 1:
        if (func_80022E44(self) != 0) {
            func_8003F78C(self);
        }
        break;
    case 2:
        if (func_80022E44(self) != 0) {
            if (GAME.selectedArea == 4) {
                D_8009CEFB++;
                func_80126760(self[3]);
                if (GAME_SELECTED_SECTION < 4) {
                    sfx = 0xF7;
                } else {
                    sfx = 0xF9;
                }
            } else {
                sfx = 0x34;
            }
            playSFX(sfx);
            func_800E98A4(self, *(s16*)(self + 0x12), *(s16*)(self + 0x16), *(s16*)(self + 0x1A));
            if (GAME.selectedArea == 0) {
                D_8009D2AE |= 1 << self[3];
                obj = allocObjectLayer2();
                if (obj != NULL) {
                    obj[0] = 1;
                    obj[2] = 3;
                    obj[3] = self[3];
                    obj[0xC] = self[0xC];
                    *(VEC3*)(obj + 0x10) = *(VEC3*)(self + 0x10);
                    obj[0x6B] = self[0x6B];
                    obj[0x1D] = self[0x1D];
                    *(s16*)(obj + 0x7A) = 0;
                    obj[4] = 0;
                    obj[5] = 0;
                    obj[6] = 0;
                }
            }
            self[4] = 3;
        }
        break;
    case 3:
        freeObjectLayer1(self);
        break;
    }
}

void applyItemEffect(GameObject* arg0, int arg1, short arg2, short arg3, int arg4);

void initItemObject(GameObject* arg0);

void rewardNone(GameObject* arg0);

void rewardItem(GameObject* arg0);

void rewardHeart(GameObject* arg0);

void rewardEffectOnly(GameObject* arg0);

void rewardBakedYam(GameObject* arg0);

void rewardDirtyMirror(GameObject* arg0);

void rewardVitalityMaxUp(GameObject* arg0);

void rewardWoodBoomerang(GameObject* arg0);

void rewardStoneBoomerang(GameObject* arg0);

void rewardIronBoomerang(GameObject* arg0);

void rewardOneUp(GameObject* arg0);

void rewardGoldenBowl(GameObject* arg0);

void rewardBitingPlantFlower(GameObject* arg0);

void rewardGrapple(GameObject* arg0);

void rewardGrappleJack(GameObject* arg0);

void rewardCrystalBalls(GameObject* arg0);

void rewardMysteriousMushroom(GameObject* arg0);

void rewardFlowerSeeds(GameObject* arg0);

void rewardPigBag(GameObject* arg0);

void rewardConditionalItem(GameObject* arg0);

void rewardJewel(GameObject* arg0);

void rewardSafeMushroom(GameObject* arg0);

void rewardAnimalDash(GameObject* arg0);

void rewardPants(GameObject* arg0);
