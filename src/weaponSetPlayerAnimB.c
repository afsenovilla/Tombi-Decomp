// FUNC 800312e8 224 MAIN0
// MATCHING 800312e8 224
// VERSION SLPS_01144 slps01144 8003229C 268
#if defined(VERSION_SLPS_01144)
#include "raw7.h"
extern void *PLAYER_ANIM;
extern char D_80010F90[], D_80010F98[], D_80010FA0[], D_80011080[], D_80011088[], D_80011090[], D_800114F4[];
void func_8003229C(char *o)
{
    switch (U8(o, 5)) {
    case 0:
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = D_80010F90; break;
        case 4: case 5: PLAYER_ANIM = D_80010F98; break;
        case 6: case 7: PLAYER_ANIM = D_80010FA0;
        }
        break;
    case 1:
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = D_80011080; break;
        case 4: case 5: PLAYER_ANIM = D_80011088; break;
        case 6: case 7: PLAYER_ANIM = D_80011090;
        }
        break;
    case 2:
        PLAYER_ANIM = D_800114F4;
        break;
    }
}
#else
#include "raw7.h"
extern void *PLAYER_ANIM;
extern char a0[], a1[], a2[], b0[], b1[], b2[];
void weaponSetPlayerAnimB(char *o)
{
    int s = U8(o, 5);
    if (s != 1) {
        if (s < 2 && s == 0) {
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = a0; break;
        case 4: case 5: PLAYER_ANIM = a1; break;
        case 6: case 7: PLAYER_ANIM = a2;
        }
        }
    } else {
        switch (U16(o, 0x2e)) {
        case 0: case 1: case 2: case 3: PLAYER_ANIM = b0; break;
        case 4: case 5: PLAYER_ANIM = b1; break;
        case 6: case 7: PLAYER_ANIM = b2;
        }
    }
}
#endif
