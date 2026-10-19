// FUNC 80048cec 80 MAIN0
// MATCHING 80048cec 80
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern void pushCat2ObjectOutOfCat4Type04(void);
extern void func_8012032C(void);

void dispatchAreaCollisionHookA(void)
{
    switch (GAME.selectedArea) {
    case AREA00_VILLAGEOFALLBEGINNINGS: pushCat2ObjectOutOfCat4Type04(); break;
    case AREA03_PHOENIXMOUNTAIN: func_8012032C(); break;
    }
}
