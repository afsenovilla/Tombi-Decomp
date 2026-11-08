// VERSION SLPS_01144 slps01144 80059B38 32
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl _card_info\n.type _card_info,@function\n_card_info:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,171\n"
"	sll	$0,$0,0x0\n"
".globl _card_load\n.type _card_load,@function\n_card_load:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,172\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
