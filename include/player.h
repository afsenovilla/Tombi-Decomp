/* PLAYER (0x800A6038): the player object. A TObj (0xC0) followed by the player-only fields (0xB8 bytes, unnamed yet);
   total 0x178 like `u_char PLAYER[0x178]` of globals_game.h / `Player` of the US decomp. For sources that include tobj.h. */
#ifndef _TOMBA_PLAYER_H
#define _TOMBA_PLAYER_H
#include "tobj.h"
typedef struct Player {
    TObj obj;               /* 0x00 */
    unsigned char ext[0xB8]; /* 0xC0 */
} Player;
extern Player PLAYER;
#endif
