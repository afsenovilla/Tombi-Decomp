// FUNC 8003bf80 952 MAIN0
// MATCHING 8003bf80 952
// Ported from psx_tomba (reward.c, updateThrownObject); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"

extern int D_80077274;
extern int D_8013A44C;
extern int D_80134018;
extern u8 D_8009C263;
extern u8 D_8009C616;
extern int D_80132A00[];
extern int D_80077D6C;

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

extern unk_8007D6E0 D_8007A148[];

typedef struct {
    int unk0;
    s16 unk4;
    s16 unk6;
} unk_8007E868;

//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/reward", initItemObject);

void updateThrownObject(u8* self)
{
    u8 state;

    state = self[4];
    switch (state) {
    case 0:
        self[4] = state + 1;
        *(s16*)(self + 0x6C) = D_8007A148[self[3]].unk0;
        *(s16*)(self + 0x6E) = D_8007A148[self[3]].unk1;
        *(s16*)(self + 0x70) = D_8007A148[self[3]].unk2;
        *(s16*)(self + 0x72) = D_8007A148[self[3]].unk3;
        *(s16*)(self + 0x1E) = D_8007A148[self[3]].unk4;
        *(int*)(self + 0x3C) = SPR_DATA[D_8007A148[self[3]].unk6];
        *(s16*)(self + 0xA6) = D_8007A148[self[3]].unk5;
        *(int*)(self + 0x24) = D_8007A148[self[3]].unk8[*(s16*)(self + 0xA6)];
        switch (self[3]) {
        case 1:
            func_801306F0(0xE, 0);
            self[0xD] = 1;
            *(s16*)(self + 8) = GetClut(0xE0, 0x1F0);
            *(s16*)(self + 0x1E) = GetTPage(0, 0, 0x1C0, 0);
            break;
        case 4:
            if (self[0xD] != 0) {
                *(s16*)(self + 8) = GetClut(0x120, 0x1ED);
            }
            break;
        case 9:
            self[0xD] = 1;
            *(s16*)(self + 8) = GetClut(0xE0, 0x1F0);
            break;
        case 0xD:
            *(int*)(self + 0x24) = D_80132A00[self[0xC]];
            readAnimFrameCount(self);
            break;
        }
        *(s16*)(self + 0x98) = 3;
        *(int*)(self + 0x28) = (int)&D_80077D6C;
        self[0xA5] = 1;
        self[0xA] = 2;
        *(int*)(self + 0x8C) = 0;
        readAnimFrameCount(self);
        break;
    case 1:
        if (self[3] == 3) {
            if (*(u16*)(self + 0x2E) & 1) {
                *(int*)(self + 0x8C) = (*(int*)(self + 0x8C) - 0x20) & 0xFF;
            } else {
                *(int*)(self + 0x8C) = (*(int*)(self + 0x8C) + 0x20) & 0xFF;
            }
        } else {
            if (*(u16*)(self + 0x2E) & 1) {
                *(int*)(self + 0x8C) = (*(int*)(self + 0x8C) + 0x14) & 0xFF;
            } else {
                *(int*)(self + 0x8C) = (*(int*)(self + 0x8C) - 0x14) & 0xFF;
            }
        }
        if (*(u16*)(self + 0x2E) & 2) {
            *(int*)(self + 0x14) += 0x50000;
        } else {
            applyFrameVelocityX(self);
        }
        if ((s16)probeCollisionAtDepthB(self, *(s16*)(*(int*)(self + 0x40) + 2), *(s16*)(self + 0x16)) != 0) {
            self[0] = 2;
            self[0xA5] = 0;
            *(int*)(self + 0x8C) = (-*(s16*)0x1F80027E << 2) & 0xFF;
            self[4] = 2;
            self[5] = 0;
            self[6] = 0;
        }
        if (func_800202B4(self) == 0) {
            self[4] = 3;
        }
        break;
    case 2:
        updateItemPickupAnim((GameObject*)self);
        if (func_800202B4(self) == 0) {
            self[4] = 3;
        }
        break;
    case 3:
        freeObjectLayer1(self);
        break;
    }
}
