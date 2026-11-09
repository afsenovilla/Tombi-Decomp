// VERSION SLPS_01144 slps01144 80059B48 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl _card_load\n.type _card_load,@function\n_card_load:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,172\n"
".set at\n.set reorder\n");
#endif
