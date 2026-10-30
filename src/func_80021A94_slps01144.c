// near-twin of testSectionFlagBit (PAL), tools/jp_neartwin.py
// VERSION SLPS_01144 slps01144 80021A94 168
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
// Demo version of PAL main0 80020B64 (func_80021A94_slps01144.c): section sizes count double (PAL single).
#include "gamestate.h"
typedef struct { unsigned short a; unsigned short pad[0x118]; unsigned short bits[2]; } S;
extern unsigned short AREA_SECTION_COUNTS[];
extern S GAME_SELECTED_SECTION;

int func_80021A94_slps01144(int n)
{
    int i;
    int s = 0;
    int m;
    for (i = 0; i < GAME.selectedArea; i++) s += AREA_SECTION_COUNTS[i] * 2;
    s += GAME_SELECTED_SECTION.a * 2;
    if (n >= 32) s++;
    m = n % 32;
    return *(int *)&GAME_SELECTED_SECTION.bits[s * 2] & (1 << m);
}
#else
#include "gamestate.h"
typedef struct { unsigned short a; unsigned short pad[0x118]; unsigned short bits[2]; } S;
extern unsigned short AREA_SECTION_COUNTS[];
extern S GAME_SELECTED_SECTION;

int func_80021A94_slps01144(int n)
{
    int i;
    int s = 0;
    int m;
    for (i = 0; i < GAME.selectedArea; i++) s += AREA_SECTION_COUNTS[i];
    s += GAME_SELECTED_SECTION.a * 2;
    if (n >= 32) s++;
    m = n % 32;
    return *(int *)&GAME_SELECTED_SECTION.bits[s * 2] & (1 << m);
}
#endif
#endif
