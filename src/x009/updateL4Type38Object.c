// FUNC 800ee25c 72 X009
// MATCHING 800ee25c 72
#include "gamestate.h"
extern void l4Type38Update(void);
extern void updateApBox(void);

void updateL4Type38Object(void)
{
    if (GAME.selectedArea == 0) {
        l4Type38Update();
    } else if (GAME.selectedArea == 9) {
        updateApBox();
    }
}
