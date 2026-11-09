// FUNC 8005d1fc 12 MAIN0
// MATCHING 8005d1fc 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl _card_info\n.type _card_info,@function\n_card_info:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,171\n"
".set at\n.set reorder\n");
