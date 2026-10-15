// FUNC 8005aa74 2328 MAIN0
// MATCHING 8005aa74 2328
// HEADERS tomba24
// Ported from psx_tomba (main/game/event.c, spawnEventTitle); MIT licence of the original project. PAL adds the padding loop.
#include "spad.h"
#define SKIP_ASM
#include "common.h"
#include "game.h"


extern char D_80080410[];
static u8 *D_800806B4[20] = {
    (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410,
    (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410,
    (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410,
    (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410,
    (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410, (u8 *)D_80080410,
};
extern s16 D_800804D8[4];
extern u16 D_800804E0[8];
extern unk_80077720 D_800804EC[6];
extern s8 **D_8013A044, **D_8013D8A4, **D_800F0E3C, **D_8011F13C, **D_800F13CC;
extern s8 **D_801385E4, **D_801339A0, **D_80102F9C, **D_8011A000, **D_801228B8;
extern s8 **D_80118C64, **D_801184C4, **D_8012D9E0, **D_80131A78, **D_8011C208;
extern s8 **D_80119884, **D_8011A7EC, **D_80128C70, **D_8011B350, **D_8011C088;
extern s8 **D_8011CA44;
extern u8 *allocObjectLayer1(void);

void FUN_8005aa74(int event_id, int completed, int duration, int state)
{
    s16 x;
    s16 y;
    u8* src;
    u8 idx;
    s8** tbl;
    s8* p;
    u8* obj;
    int i;
    int done;
    int once;
    short n;
    u16* q;
    int temp;
    int* hdr;
    short cnt;

    if (event_id == 1) {
        duration = 1;
        x = D_800A53AA - D_800A38C2;
        y = (u16)PLAYER.obj.y.p.whole - *(u16*)0x1F8000F2;
    }
    src = *(u8**)0x1F80039C;
    src += *(u16*)(src + 8);
    idx = D_800806B4[GAME.selectedArea][event_id];
    if (idx == 0xFF) {
        q = (u16*)(src + 2);
    } else {
        q = (u16*)(src + idx * 2);
    }
    src += *q;
    i = 0;
    switch (GAME.selectedArea) {
    case 0:
        tbl = D_8013A044;
        break;
    case 1:
        tbl = D_8013D8A4;
        break;
    case 2:
        switch (D_8009BCCA) {
        case 0:
            tbl = D_800F0E3C;
            break;
        case 1:
        case 2:
        case 4:
        case 5:
            tbl = D_8011F13C;
            break;
        case 3:
            tbl = D_800F13CC;
            break;
        }
        break;
    case 3:
        tbl = D_801385E4;
        break;
    case 4:
        tbl = D_801339A0;
        break;
    case 5:
        switch (D_8009BCCA) {
        case 0:
        case 2:
            tbl = D_80102F9C;
            break;
        case 1:
        case 3:
            tbl = D_8011A000;
            break;
        }
        break;
    case 6:
        tbl = D_801228B8;
        break;
    case 7:
        tbl = D_80118C64;
        break;
    case 8:
        switch (D_8009BCCA) {
        case 0:
        case 2:
            tbl = D_80102F9C;
            break;
        case 1:
        case 3:
            tbl = D_801184C4;
            break;
        }
        break;
    case 9:
        tbl = D_8012D9E0;
        break;
    case 10:
        tbl = D_80131A78;
        break;
    case 11:
        tbl = D_8011C208;
        break;
    case 12:
        tbl = D_80119884;
        break;
    case 13:
        tbl = D_8011A7EC;
        break;
    case 14:
        tbl = D_80128C70;
        break;
    case 16:
        tbl = D_8011B350;
        break;
    case 17:
        tbl = D_8011C088;
        break;
    case 18:
        tbl = D_8011CA44;
        break;
    case 19:
        switch (D_8009BCCA) {
        case 0:
            tbl = D_800F0E3C;
            break;
        case 1:
            tbl = D_8011F13C;
            break;
        case 2:
            tbl = D_80102F9C;
            break;
        }
        break;
    }
    done = 0;
    p = tbl[event_id];
    D_8009BC9B = 1;
    once = 0;
    cnt = 0;
    for (; p[0] != -1; p += 4) {
        obj = (u8*)allocObjectLayer1();
        if (obj != NULL) {
            obj[0] = 1;
            obj[2] = 1;
            obj[0xD] = 1;
            *(s16*)(obj + 8) = 0x7D16;
            obj[3] = completed;
            *(s16*)(obj + 0x1E) = 0;
            *(s8*)(obj + 0xF) = -0x5A;
            *(s16*)(obj + 0x2E) = 1;
            temp = obj[0x1C] | 0x80;
            obj[0x1C] = temp;
            *(s16*)(obj + 0x20) = duration;
            *(int*)(obj + 0xA0) = (int)(*(char *volatile *)&(*(char **)&SPAD.p33c) + *(int *)(*(char *volatile *)&(*(char **)&SPAD.p33c) + 4));
            cnt++;
            if (event_id == 1) {
                *(int*)(obj + 0x10) = (x - 0xA0) << 16;
                *(int*)(obj + 0x14) = y << 16;
                *(int*)(obj + 0x18) = (PLAYER.obj.z.p.whole + 10) << 16;
                *(int*)(obj + 0x30) = (x - 0xA0) << 16;
                *(int*)(obj + 0x34) = y << 16;
                *(int*)(obj + 0x38) = (PLAYER.obj.z.p.whole + 10) << 16;
            } else {
                *(int*)(obj + 0x10) = D_800804D8[1] << 16;
                *(int*)(obj + 0x14) = D_800804D8[2] << 16;
                *(int*)(obj + 0x18) = 0;
                *(int*)(obj + 0x30) = D_800804D8[1] << 16;
                *(int*)(obj + 0x34) = D_800804D8[2] << 16;
                *(int*)(obj + 0x38) = 0;
            }
            obj[0xC] = i;
            n = p[3] & 0xF;
            *(u16*)(obj + 0xBC) = *(u16*)src & 0xFFF;
            *(u16*)(obj + 0xC0) = D_800804E0[n];
            *(s16*)(obj + 0xC4) = p[1] * 10;
            *(s16*)(obj + 0xC6) = p[2] * 14;
            *(u16*)(obj + 0xC8) = D_800804EC[n].unk0;
            *(u16*)(obj + 0xCA) = D_800804EC[n].unk2;
            i++;
            *(u16*)(obj + 0xCC) = (u8)p[3];
            *(u16*)(obj + 0xCE) = 0;
            src += 2;
            if (D_8009C618[0] != 3 && done == 0) {
                if (state != 0) {
                    *(u16*)(obj + 0xD0) = D_8009BC98[0xF];
                    D_8009BC98[0xF] = 1;
                    D_8009BCAA = 1;
                    if (state != 4 && (PLAYER.obj.unk9E == 0 || ((u8 *)&PLAYER)[0xAC] < 2)) {
                        PLAYER.obj.state = 5;
                        PLAYER.obj.subState = 0;
                        PLAYER.obj.step = 0;
                        PLAYER.obj.unk7 = 0;
                    } else if (D_8009BCA8 == 0) {
                        D_8009BCA6 = 1;
                    }
                    *(u16*)(obj + 0xCE) = state;
                }
                *(u16*)(obj + 0xCE) |= 0x8000;
                done = 1;
            }
            if (event_id == 0xF && completed == 1 && once == 0) {
                once = 1;
                *(u16*)(obj + 0xD2) = 0x27;
                *(u16*)(obj + 0xCE) = 0x8002;
            } else {
                *(u16*)(obj + 0xD2) = 0;
            }
        }
    }
    if (cnt < 6 && completed == 1) {
        for (; cnt < 6; cnt++) {
            obj = (u8*)allocObjectLayer1();
            if (obj != NULL) {
                obj[0] = 2;
                obj[2] = 1;
                obj[0xD] = 1;
                *(s16*)(obj + 8) = 0x7D16;
                obj[3] = completed;
                *(s16*)(obj + 0x1E) = 0;
                *(s8*)(obj + 0xF) = -0x5A;
                *(s16*)(obj + 0x2E) = 1;
                temp = obj[0x1C] | 0x80;
                obj[0x1C] = temp;
                *(s16*)(obj + 0x20) = duration;
                *(int*)(obj + 0xA0) = (int)(*(char *volatile *)&(*(char **)&SPAD.p33c) + *(int *)(*(char *volatile *)&(*(char **)&SPAD.p33c) + 4));
                if (event_id == 1) {
                    *(int*)(obj + 0x10) = (x - 0xA0) << 16;
                    *(int*)(obj + 0x14) = y << 16;
                    *(int*)(obj + 0x18) = (PLAYER.obj.z.p.whole + 10) << 16;
                    *(int*)(obj + 0x30) = (x - 0xA0) << 16;
                    *(int*)(obj + 0x34) = y << 16;
                    *(int*)(obj + 0x38) = (PLAYER.obj.z.p.whole + 10) << 16;
                } else {
                    *(int*)(obj + 0x10) = D_800804D8[1] << 16;
                    *(int*)(obj + 0x14) = D_800804D8[2] << 16;
                    *(int*)(obj + 0x18) = 0;
                    *(int*)(obj + 0x30) = D_800804D8[1] << 16;
                    *(int*)(obj + 0x34) = D_800804D8[2] << 16;
                    *(int*)(obj + 0x38) = 0;
                }
                obj[0xC] = i;
                n = cnt & 0xF;
                *(u16*)(obj + 0xC0) = D_800804E0[n];
                *(s16*)(obj + 0xC4) = 0;
                *(s16*)(obj + 0xC6) = 0;
                *(u16*)(obj + 0xC8) = D_800804EC[n].unk0;
                *(u16*)(obj + 0xCA) = D_800804EC[n].unk2;
                *(u16*)(obj + 0xCC) = cnt | 0x80;
                *(u16*)(obj + 0xCE) = 0;
                i++;
                if (D_8009C618[0] != 3 && done == 0) {
                    if (state != 0) {
                        *(u16*)(obj + 0xD0) = D_8009BC98[0xF];
                        D_8009BC98[0xF] = 1;
                        D_8009BCAA = 1;
                        if (state != 4 && (PLAYER.obj.unk9E == 0 || ((u8 *)&PLAYER)[0xAC] < 2)) {
                            PLAYER.obj.state = 5;
                            PLAYER.obj.subState = 0;
                            PLAYER.obj.step = 0;
                            PLAYER.obj.unk7 = 0;
                        } else if (D_8009BCA8 == 0) {
                            D_8009BCA6 = 1;
                        }
                        *(u16*)(obj + 0xCE) = state;
                    }
                    *(u16*)(obj + 0xCE) |= 0x8000;
                    done = 1;
                }
                if (event_id == 0xF && completed == 1 && once == 0) {
                    once = 1;
                    *(u16*)(obj + 0xD2) = 0x27;
                    *(u16*)(obj + 0xCE) = 0x8002;
                } else {
                    *(u16*)(obj + 0xD2) = 0;
                }
            }
        }
    }
}
