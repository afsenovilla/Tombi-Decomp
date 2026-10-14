// FUNC 8003a6b0 108 MAIN0
// MATCHING 8003a6b0 108
/* script op: set object byte +0x6b (psx_tomba scriptop.c op94 of execGameOpcode, a separate function here). `register asm("$6")` pins the script context; without it the compiler swaps $a1/$a2 (debt). */
#include "player.h"
typedef struct { char pad[0x8a]; unsigned short w8a; char pad2[0x1190 - 0x8c]; int d1190; int d1194; } G;
typedef struct { char pad[0x6b]; unsigned char b6b; } Q;
extern G *SCRIPT_CTX;
extern Q *SCRIPT_OBJECTS[];
extern signed char D_8009D2B0;

void scriptOpSetObjField6B(void)
{
    register G *g asm("$6") = SCRIPT_CTX;
    Q *o = SCRIPT_OBJECTS[g->d1190];
    if ((D_8009D2B0 != 3 || PLAYER.obj.b6b == 1) && o != 0) o->b6b = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
}
