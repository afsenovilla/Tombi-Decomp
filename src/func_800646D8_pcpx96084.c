// VERSION PCPX_96084 tomba 800646D8 124
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Loads the current object's matrix into the GTE and draws the model through func_800294E8.
#include "common.h"
#include "psyq/libgte.h"
#include "psyq/gtemac.h"
#include "psyq/inline_c.h"
extern MATRIX *D_800DE544;
extern char *D_800E2B20;
extern void func_800294E8(int, void *, void *, int);

void func_800646D8_pcpx96084(char *o)
{
    gte_SetRotMatrix(D_800DE544);
    gte_SetTransMatrix(D_800DE544);
    func_800294E8(*(int *)(o + 0x9C), D_800E2B20 + 16, o, 1);
}
#endif
