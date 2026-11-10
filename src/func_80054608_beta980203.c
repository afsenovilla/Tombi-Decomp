// VERSION BETA_980203 beta980203 80054608 600
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_BETA_980203)
// Beta version of USA func_80055BA0 (src/func_80054608_beta980203.c) without the D_8009C618 != 3 test (fall-off-the-map check).
// Portado de psx_tomba (main/game/actor1.c, func_80055BA0); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
#include "game/area_module.h"

// INCLUDE_ASM("nonmatchings/main/game/actor1", func_80055BA0);
void func_80054608_beta980203(void)
{
    s16 *p;
    GameObject *obj;

    if (SPAD->debugNoAreaUpdate != 0) {
        return;
    }
    if (*(s32 *)&GAME == 6) {
        func_8011D858();
        return;
    }
    p = &D_80114670[(s16)D_800A53BC[0][1] * 4];
    D_800A5404[0] = *p++;
    D_800A5406[0] = *p++;
    D_800A5408_S16Arr[0] = p[0];
    D_800A540A[0] = p[1];
    func_80057044();
    func_80056E00();
    func_80057188();
    obj = &PLAYER.obj;
    switch (obj->active) {
    case 1:
    case 2:
    case 3:
    case 7:
        switch (obj->unk9E) {
        case 0:
        case 5:
        case 6:
            func_80054D60(obj);
        }
        func_80055A44(obj);
        if (D_8009BCAA | D_8009BCA7) {
            break;
        }
        func_8005548C(obj);
        goto check;
    case 4:
    case 6:
        func_80055A44(obj);
        if (D_8009BCAA | D_8009BCA7) {
            break;
        }
    check:
        if (SPAD->debugInvincible == 0 && (u16)SPAD->currentTask->state2 == 1 &&
            D_800A38EA + 0x98 < obj->y.p.whole) {
            obj->active = 5;
            setObjState(obj, 2, 3, 0);
        }
    case 0:
    default:
        if (!(D_8009BCAA | D_8009BCA7) && ((u16 *)&GAME)[0] == 0 && D_8009BCCA == 2) {
            func_80125AB4(obj);
        }
    }
    func_80053F08();
    func_80053DA0();
    resolveWeaponActorContacts();
    func_800574BC();
}
#endif
