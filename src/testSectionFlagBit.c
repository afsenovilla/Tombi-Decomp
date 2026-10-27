// FUNC 80020b64 160 MAIN0
// MATCHING 80020b64 160
// VERSION SLPM_80165 slpm80165 80021204 164
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80020B64 (testSectionFlagBit.c): section sizes count double (PAL single).
#include "gamestate.h"
typedef struct { unsigned short a; unsigned short pad[0x118]; unsigned short bits[2]; } S;
extern unsigned short AREA_SECTION_COUNTS[];
extern S GAME_SELECTED_SECTION;

int testSectionFlagBit(int n)
{
    int i;
    int s = 0;
    int m;
    for (i = 0; i < GAME.selectedArea; i++) s += AREA_SECTION_COUNTS[i] * 2;
    s += GAME_SELECTED_SECTION.a;
    if (n >= 32) s++;
    m = n % 32;
    return *(int *)&GAME_SELECTED_SECTION.bits[s * 2] & (1 << m);
}
#else
#include "gamestate.h"
typedef struct { unsigned short a; unsigned short pad[0x118]; unsigned short bits[2]; } S;
extern unsigned short AREA_SECTION_COUNTS[];
extern S GAME_SELECTED_SECTION;

int testSectionFlagBit(int n)
{
    int i;
    int s = 0;
    int m;
    for (i = 0; i < GAME.selectedArea; i++) s += AREA_SECTION_COUNTS[i];
    s += GAME_SELECTED_SECTION.a;
    if (n >= 32) s++;
    m = n % 32;
    return *(int *)&GAME_SELECTED_SECTION.bits[s * 2] & (1 << m);
}
#endif
