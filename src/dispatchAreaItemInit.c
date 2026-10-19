// FUNC 8003e8d0 72 MAIN0
// MATCHING 8003e8d0 72
#define SKIP_ASM
#include "common.h"
#include "game.h"
void func_80123344(void);
void func_8011E154(void);

void dispatchAreaItemInit(void)
{
    unsigned short v = GAME.selectedArea;
    if (v == AREA00_VILLAGEOFALLBEGINNINGS) {
        func_80123344();
    } else if (v == AREA04_HAUNTEDMANSION) {
        func_8011E154();
    }
}
