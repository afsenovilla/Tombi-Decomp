// FUNC 80116764 60 X006
// MATCHING 80116764 60
#include "gamestate.h"
extern void updateAreaScroll(void);
extern void a06SectionScrollStep(void);

void a06UpdateScroll(void)
{
    if (GAME.selectedSection) updateAreaScroll();
    else a06SectionScrollStep();
}
