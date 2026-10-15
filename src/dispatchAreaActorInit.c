// FUNC 800452ac 152 MAIN0
// MATCHING 800452ac 152
#include "gamestate.h"
void playerContactCat2Type00(void);
void func_801240A8(void);
void func_8011F8CC(void);
void func_8011EFA4(void);
void dispatchAreaActorInit(void)
{
    switch (GAME.selectedArea) {
    case 0: playerContactCat2Type00(); break;
    case 1: func_801240A8(); break;
    case 3: func_8011F8CC(); break;
    case 4: func_8011EFA4(); break;
    }
}
