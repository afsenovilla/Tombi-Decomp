// FUNC 80048cec 80 MAIN0
// MATCHING 80048cec 80
#include "gamestate.h"
extern void pushCat2ObjectOutOfCat4Type00(void);
extern void func_8012032C(void);

void dispatchAreaCollisionHookA(void)
{
    switch (GAME.selectedArea) {
    case 0: pushCat2ObjectOutOfCat4Type00(); break;
    case 3: func_8012032C(); break;
    }
}
