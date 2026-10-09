// FUNC 8011ccd8 2332 X010
/* not started (score: everything). Real extent is 0x8011CCD8..0x8011D5F4 (2332 B): the csv pieces 8011CCD8 (1628)
   and 8011D334 (184) plus the tail up to 8011D5F0 are one function (state 0 falls into state 1).
   Notes from the asm: o = s3, P = *(TObj **)0x8009F0EC (held object), Q = *(short **)0x8009C330 (Q[1] flag);
   state 0: PlayerSetAnimIfChanged(o, 10), zero d8c/wb6/wb2/velH/velV, baa = 1, place o next to P (wb8/wba
   offsets, +-8 by animFrame), FUN_8001e5f4(4, 0x7f), state++; state 1: AnimAdvance, height s0 from
   rsin(angle of P->d30 by P->w7a) * (P->box0 - 0x24) >> 12, pad (0x8009D670) bits 0x50/0x10/0x40 pick
   PlayerSetAnimIfChanged 0xc/0x2d and +-0x18000 on y with FUN_800408d8 and FUN_8001e560(0x1d, 0) every 16
   frames, func_8011E000(o, P, 2|1, angle), landing test (b69 or FUN_80040278) -> d8c = D_801152E8[wb0] and
   reset, then a rcos/rsin rotation per P->w7a into s4/s0, wb8/wbc-like targets at 0xe8/0xea, func_8011EF98,
   and the 0x1F8001FC & 0x1F8003C6 release block (D_8009D2B0 = 0, D_8009C984 & 0x40 -> a7 from pad 0x3c4). */
#include "TOBJ.H"

void func_8011CCD8(TObj *o)
{
}
