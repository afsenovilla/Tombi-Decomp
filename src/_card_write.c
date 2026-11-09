// FUNC 8005d254 12 MAIN0
// MATCHING 8005d254 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl _card_write\n.type _card_write,@function\n_card_write:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,78\n"
".set at\n.set reorder\n");
