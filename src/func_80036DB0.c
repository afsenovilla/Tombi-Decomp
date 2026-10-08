// FUNC 8003395c 72 MAIN0
// MATCHING 8003395c 72
// Ported from psx_tomba (objlogic.c, func_80036DB0); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s16 func_80036DB0(void)
{
    s16 r = 0;

    switch (D_800A539D) {
    case 5:
    case 6:
    case 7:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 21:
    case 22:
    case 23:
    case 27:
    case 29:
    case 30:
    case 32:
    case 33:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 46:
    case 47:
    case 48:
    case 51:
    case 52:
    case 53:
    case 54:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 69:
        r++;
    }
    return r;
}
