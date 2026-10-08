// FUNC 80045580 128 MAIN0
// MATCHING 80045580 128
// Ported from psx_tomba (actor1.c, dispatchAreaActorUpdate); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void dispatchAreaActorUpdate(void)
{
    switch (GAME.selectedArea) {
    case AREA00_VILLAGEOFALLBEGINNINGS:
        func_801248A0();
        break;
    case AREA04_HAUNTEDMANSION:
        func_8011E3E4();
        break;
    case AREA10_DEEPJUNGLE:
        func_8011FC7C();
        break;
    }
}
