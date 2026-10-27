// FUNC 8011bc34 120 X006
// MATCHING 8011bc34 120
// VERSION SLPM_80165 slpm80165 800474EC 100
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x006 8011BC34 (func_8011BC34.c): no GVARS.playerLanded guard, tests the whole flag byte.
#include "gvars.h"

typedef struct {
    unsigned char b0;
    unsigned char pad[7];
    short w8;
} S;

extern S HUD_STATE;
extern void func_8011BCAC(S *);

void func_8011BC34(void)
{
    S *p = &HUD_STATE;

    if (GVARS.b3f) HUD_STATE.w8 = 0x78;
    if (HUD_STATE.w8 == 0 && p->b0) func_8011BCAC(p);
}
#else
#include "gvars.h"

typedef struct {
    unsigned char b0;
    unsigned char pad[7];
    short w8;
} S;

extern S HUD_STATE;
extern void drawRaceHud(S *);

void drawRaceHudIfActive(void)
{
    S *p = &HUD_STATE;

    if (GVARS.playerLanded) {
        if (GVARS.b3f) HUD_STATE.w8 = 0x78;
        if (HUD_STATE.w8 == 0 && (p->b0 & 1)) drawRaceHud(p);
    }
}
#endif
