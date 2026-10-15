// FUNC 80116dd0 124 X009
// MATCHING 80116dd0 124

#include "gamestate.h"
extern void updateAreaScroll(void);
extern void updateAreaScrollB(void);
extern void func_80116BD0(void);

void dispatchAreaScroll(void)
{
    switch (GAME.selectedSection) {
    case 0:
        updateAreaScroll();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        updateAreaScrollB();
        break;
    case 6:
        func_80116BD0();
        break;
    }
}
