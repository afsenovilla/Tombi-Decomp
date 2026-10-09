// FUNC 801244a0 1800 X001
/* not started. Same rotated-box collision family as wip/func_801253AC.c (start identical up to the
   rcos/rsin projections and the |c|/|s| selector; angle window here is 0x500..0xe00), 0xa0 frame with s0-s7
   saved, so it carries large locals/inlines. Draft func_801253AC first and extend it.
   Better base: 87% opcode-identical to src/x010/wip/func_8011E000.c (score 146): same code with a/b s-regs swapped
   and without the D_8009D2C3 & 0x40 tests (D_1F8001D2 test only). */
#include "TOBJ.H"

int func_801244A0(TObj *a, TObj *b, unsigned char k, int ang)
{
    return 0;
}
