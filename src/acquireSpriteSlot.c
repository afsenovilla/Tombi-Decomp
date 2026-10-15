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
