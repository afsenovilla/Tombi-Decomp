// FUNC 8005ba44 12 MAIN0
// MATCHING 8005ba44 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl nextfile\n.type nextfile,@function\nnextfile:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,67\n"
".set at\n.set reorder\n");
