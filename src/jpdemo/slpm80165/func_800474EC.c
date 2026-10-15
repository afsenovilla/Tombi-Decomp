// JPDEMO slpm80165 800474EC 100
// Demo version of PAL x006 8011BC34 (func_8011BC34.c): no GVARS.b3a guard, tests the whole flag byte.
#include "gvars.h"

typedef struct {
    unsigned char b0;
    unsigned char pad[7];
    short w8;
} S;

extern S D_800B1410;
extern void func_8011BCAC(S *);

void func_8011BC34(void)
{
    S *p = &D_800B1410;

    if (GVARS.b3f) D_800B1410.w8 = 0x78;
    if (D_800B1410.w8 == 0 && p->b0) func_8011BCAC(p);
}
