// VERSION BETA_980203 beta980203 8005786C 1496
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_BETA_980203)
// Beta version of USA func_80058E14 (src/func_80058E14_scus94236.c), same body under the beta names; portado de psx_tomba (main/game/objpool.c, func_80058E14); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "game/area_module.h"
// beta names
extern __typeof__(D_8009BCCA) D_800A2442;
extern __typeof__(D_8009C361) D_800A2AD9;
extern __typeof__(GAME) D_800A2440;
extern __typeof__(PLAYER) D_800ABB20;
extern __typeof__(cullAndQueueDraw) ObjCullRegister;
extern __typeof__(freeActorObject) freeObjectLayer2;
extern __typeof__(readAnimFrameCount) AnimLoadDuration;
extern __typeof__(tickAnimation) AnimAdvance;
#define D_80015578 D_80015238
#define D_80015590 D_80015250
#define D_800155A4 D_80015264
#define D_800155BC D_8001527C
#define D_8009BCCA D_800A2442
#define D_8009C361 D_800A2AD9
#define GAME D_800A2440
#define PLAYER D_800ABB20
#define cullAndQueueDraw ObjCullRegister
#define freeActorObject freeObjectLayer2
#define func_80057C74 eventTakePlayerControlA
#define func_800580BC eventCarryPlayerB
#define func_80058464 eventTakePlayerControlC
#define func_80058960 eventTakePlayerControlD
#define func_801156E8 func_80119C0C
#define readAnimFrameCount AnimLoadDuration
#define tickAnimation AnimAdvance
#define SETANIM(o, a, b) \
    switch ((o)->unk6A - 1) { \
    case 0: func_800E7DF8(&PLAYER.obj, a, b); break; \
    case 1: func_800E8A30(&PLAYER.obj, a, b); break; \
    case 2: func_800E8758(&PLAYER.obj, a, b); break; \
    }
#define SETANIM(o, a, b) \
    switch ((o)->unk6A - 1) { \
    case 0: func_800E7DF8(&PLAYER.obj, a, b); break; \
    case 1: func_800E8A30(&PLAYER.obj, a, b); break; \
    case 2: func_800E8758(&PLAYER.obj, a, b); break; \
    }
// INCLUDE_ASM("nonmatchings/main/game/objpool", func_80058E14);
extern void *D_80015578[];
extern void *D_80015590[];
extern void *D_800155A4[];
extern void *D_800155BC[];

void func_8005786C_beta980203(GameObject *obj)
{
    Player *pl = &PLAYER;
    s32 f;

    switch (obj->state) {
    case 0:
        if (obj->id == 0) {
            obj->state = 3;
            break;
        }
        obj->unk6A = 0;
        switch (GAME.selectedArea) {
        case 0:
            obj->tpage = 0xE;
            *(void ***)&obj->unkA8 = D_80015590;
            break;
        case 1:
            switch ((s32)D_8009BCCA) {
            case 0:
            case 1:
                obj->tpage = 0xB;
                break;
            case 2:
            case 3:
            case 4:
                obj->tpage = 6;
                break;
            }
            *(void ***)&obj->unkA8 = D_80015590;
            break;
        case 2:
            switch ((s32)D_8009BCCA) {
            case 0:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_800155BC;
                obj->unk6A = 1;
                break;
            case 3:
                obj->tpage = 0x10;
                *(void ***)&obj->unkA8 = D_800155A4;
                obj->unk6A = 3;
                break;
            case 4:
            case 5:
                obj->tpage = 0x11;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            case 1:
            case 2:
                obj->tpage = 0xE;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            }
            break;
        case 3:
            switch ((s32)D_8009BCCA) {
            case 0:
            case 4:
                obj->tpage = 0xD;
                break;
            case 1:
            case 2:
            case 3:
            case 5:
            case 6:
                obj->tpage = 5;
                break;
            }
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 4:
            obj->tpage = 0x10;
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 5:
            switch ((s32)D_8009BCCA) {
            case 0:
            case 2:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_800155BC;
                obj->unk6A = 2;
                break;
            case 1:
            case 3:
                obj->tpage = 0xB;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            }
            break;
        case 6:
            obj->tpage = 0xF;
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 8:
            switch ((s32)D_8009BCCA) {
            case 0:
            case 2:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_800155BC;
                obj->unk6A = 2;
                break;
            case 1:
            case 3:
                obj->tpage = 0xE;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            }
            break;
        case 9:
            switch ((s32)D_8009BCCA) {
            case 0:
                obj->tpage = 0xC;
                *(void ***)&obj->unkA8 = D_80015590;
                break;
            case 1:
                obj->tpage = 0x12;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            case 2:
                obj->tpage = 0x11;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            case 3:
                obj->tpage = 0xB;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            case 4:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            case 5:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            }
            break;
        case 10:
            switch ((s32)D_8009BCCA) {
            case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                obj->tpage = 7;
                break;
            case 8:
                obj->tpage = 0xC;
                break;
            }
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 11:
            obj->tpage = 0xD;
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 13:
            obj->tpage = 0xD;
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 14:
            obj->tpage = 0xA;
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 16:
            switch ((s32)D_8009BCCA) {
            case 4: case 5: case 6:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            }
            break;
        case 17:
            obj->tpage = 0xB;
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 18:
            switch ((s32)D_8009BCCA) {
            case 1:
                obj->tpage = 0xA;
                break;
            case 2:
                obj->tpage = 9;
                break;
            }
            *(void ***)&obj->unkA8 = D_80015578;
            break;
        case 19:
            switch ((s32)D_8009BCCA) {
            case 0:
                obj->tpage = 0xF;
                *(void ***)&obj->unkA8 = D_800155BC;
                obj->unk6A = 1;
                break;
            case 1:
                obj->tpage = 0xE;
                *(void ***)&obj->unkA8 = D_80015578;
                break;
            case 2:
                obj->tpage = 0x12;
                *(void ***)&obj->unkA8 = D_800155A4;
                obj->unk6A = 2;
                break;
            }
            break;
        }
        obj->category |= 0x80;
        obj->state++;
        f = pl->obj.priority;
        obj->drawType = DRAW_SPRITE_ROTATED;
        obj->animFrame = 0;
        obj->rotX = 0;
        obj->rotY = 0;
        obj->rotZ = 0;
        setHitbox(obj, 0x10, 0x10, 0x20, 0x20);
        obj->drawFlags = 0x81;
        obj->priority = f + 1;
        obj->clut = GetClut(0x80, 0x1F9);
        obj->spriteBank = SPR_DATA[15];
        obj->unkAC = 3;
        obj->anim = (*(void ***)&obj->unkA8)[3];
        readAnimFrameCount(obj);
        if (obj->id == 1) {
            obj->x.p.whole = pl->obj.x.p.whole;
            obj->y.p.whole = pl->obj.y.p.whole;
            obj->z.p.whole = pl->obj.z.p.whole;
        }
        if (*(s32 *)&GAME == 0x1000D) {
            obj->state = 4;
        }
        break;
    case 1:
        cullAndQueueDraw(obj);
        if (obj->id == 0) {
            tickAnimation(obj);
            break;
        }
        switch (obj->unk6A) {
        case 0:
            switch (D_8009C361) {
            case 0:
                func_80057C74(obj);
                break;
            case 1:
                func_800580BC(obj);
                break;
            }
            break;
        case 1:
        case 2:
        case 3:
            switch (D_8009C361) {
            case 0:
                func_80058464(obj);
                break;
            case 1:
                func_80058960(obj);
                break;
            }
            break;
        }
        break;
    case 2:
    case 3:
        freeActorObject(obj);
        break;
    case 4:
        func_801156E8(obj);
        break;
    }
}
#endif
