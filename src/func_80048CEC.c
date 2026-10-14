// FUNC 80048cec 80 MAIN0
// MATCHING 80048cec 80
#include "gamestate.h"
extern void func_80126CA4(void);
extern void func_8012032C(void);

void func_80048CEC(void)
{
    switch (GAME.selectedArea) {
    case 0: func_80126CA4(); break;
    case 3: func_8012032C(); break;
    }
}
