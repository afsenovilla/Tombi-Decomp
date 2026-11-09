// FUNC 8005d340 12 MAIN0
// MATCHING 8005d340 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl InitCARD2\n.type InitCARD2,@function\nInitCARD2:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,74\n"
".set at\n.set reorder\n");
