// VERSION PCPX_96084 tomba 8005A55C 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of drawGameplayScene (src/drawGameplayScene.c): light matrix from the scratchpad (0x1F800324)
// loaded inline, the draw lists, the pause menu when frozen with D_800DE4A0 == 1, then the HUD.
#include "common.h"
#include "psyq/libgte.h"
#include "psyq/gtemac.h"
#include "psyq/inline_c.h"
extern void func_8005A624_pcpx96084(void), func_8005C228(void), func_80060E78_pcpx96084(void), func_80061004(void);
extern void drawCategory5Objects(void), func_800636B0(void), drawCategory4Objects(void), drawCategory8Objects(void);
extern void func_80064834(void), func_80065B34(void), func_80066298_pcpx96084(void);
extern short D_1F800126;
extern MATRIX D_1F800324;
extern signed char D_800DE4A0;

void func_8005A55C(void)
{
    gte_SetLightMatrix(&D_1F800324);
    func_8005A624_pcpx96084();
    func_8005C228();
    func_80060E78_pcpx96084();
    func_80061004();
    drawCategory5Objects();
    func_800636B0();
    drawCategory4Objects();
    drawCategory8Objects();
    func_80064834();
    if (D_1F800126 == 1 && D_800DE4A0 == D_1F800126)
        func_80065B34();
    func_80066298_pcpx96084();
}
#endif
