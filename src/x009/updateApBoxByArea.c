// FUNC 800ee25c 72 X009
// MATCHING 800ee25c 72
#include "gamestate.h"
extern void updateA00ApBox100k(void);
extern void updateApBox(void);

void updateApBoxByArea(void)
{
    if (GAME.selectedArea == 0) {
        updateA00ApBox100k();
    } else if (GAME.selectedArea == 9) {
        updateApBox();
    }
}
