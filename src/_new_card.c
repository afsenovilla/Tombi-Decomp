// FUNC 8005d264 12 MAIN0
// MATCHING 8005d264 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl _new_card\n.type _new_card,@function\n_new_card:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,80\n"
".set at\n.set reorder\n");
