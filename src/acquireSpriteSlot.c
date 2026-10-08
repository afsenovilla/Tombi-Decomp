// FUNC 8005b38c 664 MAIN0
// MATCHING 8005b38c 664
// Ported from psx_tomba (event.c, acquireSpriteSlot); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
typedef struct {
    u16 unk0;
    u16 unk2;
} unk_80077720;

u8 D_80077428[0xC8] = {
    0x02, 0x03, 0x04, 0x06, 0x08, 0x09, 0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x48, 0x0E, 0x0F, 0x13, 0x14,
    0x15, 0x16, 0xFF, 0x1C, 0x1E, 0x1F, 0x20, 0x22, 0x27, 0x28, 0x1A, 0x2F, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x37, 0x38, 0x39, 0x3D, 0x3E, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0xFF, 0x46, 0x47,
    0xFF, 0x49, 0x4A, 0x4B, 0xFF, 0x4C, 0x4F, 0x50, 0x51, 0x52, 0x53, 0xFF, 0xFF, 0xFF, 0x59, 0x56,
    0x58, 0x5B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x5C, 0xFF, 0xFF, 0xFF, 0x5F, 0x57, 0x62,
    0x19, 0x71, 0x81, 0xFF, 0xFF, 0x55, 0x23, 0x24, 0x26, 0xFF, 0x6C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x5D, 0x64, 0x63, 0x5E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x4D,
    0x69, 0x25, 0x73, 0x2C, 0x68, 0xFF, 0x60, 0x6B, 0x72, 0x75, 0xFF, 0x65, 0x66, 0x79, 0x67, 0x6E,
    0x7B, 0x80, 0xFF, 0xFF, 0x74, 0x76, 0x77, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x7C, 0x7A, 0x7D, 0xFF, 0xFF, 0xFF, 0x61, 0x82, 0xFF, 0x6A, 0xFF, 0xFF, 0x6D, 0x84, 0xFF, 0x83,
    0x85, 0x7F, 0xFF, 0xFF, 0x86, 0xFF, 0x05, 0x07, 0x18, 0x1B, 0x3A, 0x3F, 0x36, 0x3B, 0x4E, 0x2D,
    0x2E, 0x10, 0x11, 0x12, 0x17, 0x21, 0x1D, 0x54, 0xFF, 0x3C, 0xFF, 0x29, 0x2A, 0x2B, 0x6F, 0x70,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

s16 D_800774F0[4] = { 0, 0xB4, 0, 0 };

u16 D_800774F8[8] = { 0, 1, 2, 3, 4, 5, 6, 0 };

unk_80077720 D_80077508[6] = {
    { 0xFFD8, 0 },
    { 0xFFEC, 0 },
    { 0, 0 },
    { 20, 0 },
    { 40, 0 },
    { 60, 0 }
};

int AP_TABLE[8] = { 0, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

u_char EVENT_STARTED_AP_TABLE[0xC8] = {
    0, 0, 1, 2, 0, 2, 0, 2, 1, 1, 0, 1, 0, 0, 1, 4, 2, 1, 0, 2,
    3, 3, 2, 3, 3, 2, 0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 0, 1, 2, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 0,
    0, 0, 2, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 2, 3, 2, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 1, 2,
    2, 2, 0, 0, 0, 1, 1, 2, 1, 2, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 2, 2, 0, 1, 0, 0, 0, 1, 0, 0,
    0, 2, 0, 0, 2, 0, 2, 3, 0, 5, 2, 2, 0, 2, 0, 1, 1, 3, 2, 2,
    1, 2, 2, 1, 0, 1, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0
};

u_char EVENT_COMPLETE_AP_TABLE[0xC8] = {
    0, 2, 2, 3, 1, 4, 0, 5, 2, 2, 1, 2, 3, 3, 3, 3, 2, 4, 0, 3,
    4, 3, 2, 4, 4, 3, 2, 4, 1, 2, 6, 3, 3, 3, 2, 4, 3, 3, 3, 4,
    4, 2, 2, 3, 2, 0, 2, 5, 0, 2, 2, 3, 0, 2, 6, 3, 3, 3, 5, 0,
    0, 0, 2, 5, 3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 2, 1, 2, 3,
    6, 4, 3, 3, 0, 6, 2, 2, 2, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 2, 3, 3, 2, 0, 0, 0, 0, 0, 6, 3, 4, 3, 2, 3, 0, 2, 3,
    3, 3, 0, 2, 6, 2, 3, 5, 3, 5, 0, 0, 3, 2, 3, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 4, 2, 2, 0, 0, 0, 0, 4, 3, 3, 0, 0, 2, 5, 0, 1,
    7, 3, 0, 0, 5, 0, 3, 2, 1, 3, 2, 3, 3, 0, 2, 2, 2, 5, 5, 2,
    4, 3, 2, 2, 0, 2, 0, 2, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0
};

u8* D_800776D0[20] = {
    D_80077428, D_80077428, D_80077428, D_80077428, D_80077428,
    D_80077428, D_80077428, D_80077428, D_80077428, D_80077428,
    D_80077428, D_80077428, D_80077428, D_80077428, D_80077428,
    D_80077428, D_80077428, D_80077428, D_80077428, D_80077428
};

unk_80077720 D_80077720[2] = {
    { 0x5A, 0xB4 },
    { 0x96, 0x5A }
};





extern u16 D_800A53AA;
extern u16 D_800A38C2;
extern u8 D_8009BC9B;
extern u8 D_8009BCA6;
extern u8 D_8009BCA8;
extern s8** D_80139330;
extern s8** D_8013CC1C;
extern s8** D_800F077C;
extern s8** D_80137998;
extern s8** D_80132CAC;
extern s8** D_80119334;
extern s8** D_80121BE0;
extern s8** D_80118018;
extern s8** D_801177F8;
extern s8** D_8012CB9C;
extern s8** D_80130DD8;
extern s8** D_8011B4C8;
extern s8** D_80118C38;
extern s8** D_80119B44;
extern s8** D_80128018;
extern s8** D_8011A5CC;
extern s8** D_8011B2A8;
extern s8** D_8011BDC0;
extern s8** D_800F00BC;
extern s8** D_8011E3A0;
extern s8** D_80102000;

short acquireSpriteSlot(int id)
{
    RECT rect;
    int i;
    int freeSlot;
    int j;
    int k;
    short slot;
    u16* src;
    u16* dst;
    u16 w;
    u16 h;
    u16 a;
    u16 b;
    DR_LOAD* load;

    freeSlot = -1;
    for (i = 0; i < 0x30; i++) {
        if (SPRITE_SLOTS[i].id == id) {
            SPRITE_SLOTS[i].refCount++;
            return i;
        }
        if (SPRITE_SLOTS[i].id == -1) {
            freeSlot = i;
        }
    }

    slot = freeSlot;
    if (slot == -1) {
        return -1;
    }

    SPRITE_SLOTS[slot].refCount = 1;
    src = ((scratchpad*)PSX_SCRATCH)->unk39C;
    dst = (u16*)(id * 2 + (int)src);
    src = (u16*)((u8*)src + dst[0x48]);
    w = *src++;
    h = *src++;
    a = *src++;
    b = *src++;
    SPRITE_SLOTS[slot].id = id;
    SPRITE_SLOTS[slot].val[4] = 0;
    SPRITE_SLOTS[slot].val[2] = a;
    SPRITE_SLOTS[slot].val[3] = h;
    SPRITE_SLOTS[slot].val[5] = b;
    for (k = 0; k < 4; k++) {
        load = (DR_LOAD*)D_1F800164;
        setRECT(&rect, SPRITE_SLOTS[slot].val[0], SPRITE_SLOTS[slot].val[1] + k * (h >> 2), w, h >> 2);
        SetDrawLoad(load, &rect);
        dst = (u16*)load->p;
        for (j = 0; j < w * (u16)(h >> 2); j++) {
            *dst++ = *src++;
        }
        addPrim(D_1F8001E0 + 4, load);
        D_1F800164 += sizeof(DR_LOAD);
    }
    return freeSlot;
}
