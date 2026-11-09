// FUNC 8005ba24 12 MAIN0
// MATCHING 8005ba24 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl close\n.type close,@function\nclose:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,54\n"
".set at\n.set reorder\n");
