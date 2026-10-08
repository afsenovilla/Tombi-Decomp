// FUNC 800174c0 60 MAIN0
// MATCHING 800174c0 60
// Ported from psx_tomba (gfxinit.c, loadImageRect); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void loadImageRect(u_long* p, short x, short y, short w, short h)
{
    RECT rect;
    setRECT(&rect, x, y, w, h);
    LoadImage(&rect, p);
    return;
}
