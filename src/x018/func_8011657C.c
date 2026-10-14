// FUNC 8011657c 44 X018
// MATCHING 8011657c 44
#include "gamestate.h"
typedef struct { char p[3]; unsigned char b3; int d4; } S;

void func_8011657C(S *a, S *b)
{
    if (GAME.selectedSection == 1) {
        char *t = (char *)b + b->d4;
        a->b3 = 1;
        a->d4 = (int)t;
    }
}
