// FUNC 80045a50 152 MAIN0
// MATCHING 80045a50 152
// Ported from psx_tomba (actor1.c, dispatchAreaActorSpawn); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void dispatchAreaActorSpawn(void)
{
    switch (GAME.selectedArea) {
    case AREA00_VILLAGEOFALLBEGINNINGS:
        func_80123D24();
        break;
    case AREA03_PHOENIXMOUNTAIN:
        func_8011EF08();
        break;
    case AREA04_HAUNTEDMANSION:
        func_8011E170();
        break;
    case AREA09_MUSHROOMVILLAGE:
        func_8011F650();
        break;
    }
}
