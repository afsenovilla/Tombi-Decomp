// FUNC 8005bd14 12 MAIN0
// MATCHING 8005bd14 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl firstfile\n.type firstfile,@function\nfirstfile:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,66\n"
".set at\n.set reorder\n");
