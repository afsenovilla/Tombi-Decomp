// VERSION BETA_980203 beta980203 8001DB50 1616
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_BETA_980203)
// Beta version of USA spawnEventTitle: no 0x8000 flag on the title state (cleared in an else), the event-0xF title state is 2.
// Portado de psx_tomba (main/game/event.c, spawnEventTitle); licencia MIT del proyecto original.
#ifndef SKIP_ASM
#define SKIP_ASM
#endif
#include "common.h"
#include "game.h"
extern u8 D_80077428[0xC8];
extern s16 D_800774F0[4];
extern u16 D_800774F8[8];
extern unk_80077720 D_80077508[6];
extern int AP_TABLE[8];
extern u_char EVENT_STARTED_AP_TABLE[0xC8];
extern u_char EVENT_COMPLETE_AP_TABLE[0xC8];
extern u8* D_800776D0[20];
extern unk_80077720 D_80077720[2];
// INCLUDE_ASM("nonmatchings/main/game/event", spawnEventTitle);
int spawnEventTitle(int event_id, int completed, int duration, int state)
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
    int n;
    u16* q;
    int temp;
    int* hdr;

    if (event_id == 1) {
        duration = 1;
        x = D_800A53AA - D_800A38C2;
        y = (u16)PLAYER.obj.y.p.whole - (*(u16 *)&SPAD->vpy.p.whole);
    }
    src = (*(u8* *)&SPAD->eventData);
    src += *(u16*)(src + 8);
    idx = D_800776D0[GAME.selectedArea][event_id];
    if (idx == 0xFF) {
        q = (u16*)(src + 2);
    } else {
        q = (u16*)(src + idx * 2);
    }
    src += *q;
    i = 0;
    switch (GAME.selectedArea) {
    case 0:
        tbl = D_80139330;
        break;
    case 1:
        tbl = D_8013CC1C;
        break;
    case 2:
        switch (D_8009BCCA) {
        case 0:
            tbl = D_800F00BC;
            break;
        case 1:
        case 2:
        case 4:
        case 5:
            tbl = D_8011E3A0;
            break;
        case 3:
            tbl = D_800F077C;
            break;
        }
        break;
    case 3:
        tbl = D_80137998;
        break;
    case 4:
        tbl = D_80132CAC;
        break;
    case 5:
        switch (D_8009BCCA) {
        case 0:
        case 2:
            tbl = D_80102000;
            break;
        case 1:
        case 3:
            tbl = D_80119334;
            break;
        }
        break;
    case 6:
        tbl = D_80121BE0;
        break;
    case 7:
        tbl = D_80118018;
        break;
    case 8:
        switch (D_8009BCCA) {
        case 0:
        case 2:
            tbl = D_80102000;
            break;
        case 1:
        case 3:
            tbl = D_801177F8;
            break;
        }
        break;
    case 9:
        tbl = D_8012CB9C;
        break;
    case 10:
        tbl = D_80130DD8;
        break;
    case 11:
        tbl = D_8011B4C8;
        break;
    case 12:
        tbl = D_80118C38;
        break;
    case 13:
        tbl = D_80119B44;
        break;
    case 14:
        tbl = D_80128018;
        break;
    case 16:
        tbl = D_8011A5CC;
        break;
    case 17:
        tbl = D_8011B2A8;
        break;
    case 18:
        tbl = D_8011BDC0;
        break;
    case 19:
        switch (D_8009BCCA) {
        case 0:
            tbl = D_800F00BC;
            break;
        case 1:
            tbl = D_8011E3A0;
            break;
        case 2:
            tbl = D_80102000;
            break;
        }
        break;
    }
    done = 0;
    p = tbl[event_id];
    D_8009BC9B = 1;
    once = 0;
    for (; p[0] != -1; p += 4) {
        obj = (u8*)allocModelObject();
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
            temp = (int)&SCRATCHPAD.wmdData[3];
            hdr = *(int* volatile*)temp;
            temp = (int)*(int* volatile*)temp;
            temp += hdr[1];
            *(int*)(obj + 0xA0) = temp;
            if (event_id == 1) {
                *(int*)(obj + 0x10) = (x - 0xA0) << 16;
                *(int*)(obj + 0x14) = y << 16;
                *(int*)(obj + 0x18) = (PLAYER.obj.z.p.whole + 10) << 16;
                *(int*)(obj + 0x30) = (x - 0xA0) << 16;
                *(int*)(obj + 0x34) = y << 16;
                *(int*)(obj + 0x38) = (PLAYER.obj.z.p.whole + 10) << 16;
            } else {
                *(int*)(obj + 0x10) = D_800774F0[1] << 16;
                *(int*)(obj + 0x14) = D_800774F0[2] << 16;
                *(int*)(obj + 0x18) = 0;
                *(int*)(obj + 0x30) = D_800774F0[1] << 16;
                *(int*)(obj + 0x34) = D_800774F0[2] << 16;
                *(int*)(obj + 0x38) = 0;
            }
            obj[0xC] = i;
            n = p[3] & 0xF;
            *(u16*)(obj + 0xBC) = *(u16*)src & 0xFFF;
            *(u16*)(obj + 0xC0) = D_800774F8[n];
            *(s16*)(obj + 0xC4) = p[1] * 10;
            *(s16*)(obj + 0xC6) = p[2] * 14;
            *(u16*)(obj + 0xC8) = D_80077508[n].unk0;
            *(u16*)(obj + 0xCA) = D_80077508[n].unk2;
            *(u16*)(obj + 0xCC) = (u8)p[3];
            i++;
            src += 2;
            if (D_8009C618[0] != 3 && state != 0 && done == 0) {
                {
                    *(u16*)(obj + 0xD0) = D_8009BC98.unk0F;
                    D_8009BC98.unk0F = 1;
                    D_8009BCAA = 1;
                    if (state != 4 && (PLAYER.obj.unk9E == 0 || ((u8 *)&PLAYER)[0xAC] < 2)) {
                        PLAYER.obj.state = PLAYER_SCRIPTED;
                        PLAYER.obj.subState = PLAYER_IDLE;
                        PLAYER.obj.step = 0;
                        PLAYER.obj.unk7 = 0;
                    } else if (D_8009BCA8 == 0) {
                        D_8009BCA6 = 1;
                    }
                    *(u16*)(obj + 0xCE) = state;
                }
                done = 1;
            } else {
                *(u16*)(obj + 0xCE) = 0;
            }
            if (event_id == 0xF && completed == 1 && once == 0) {
                once = 1;
                *(u16*)(obj + 0xD2) = 0x27;
                *(u16*)(obj + 0xCE) = 2;
            } else {
                *(u16*)(obj + 0xD2) = 0;
            }
        }
    }
}
#endif
