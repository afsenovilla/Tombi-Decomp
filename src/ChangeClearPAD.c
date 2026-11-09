// FUNC 8005d330 12 MAIN0
// MATCHING 8005d330 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl ChangeClearPAD\n.type ChangeClearPAD,@function\nChangeClearPAD:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,91\n"
".set at\n.set reorder\n");
