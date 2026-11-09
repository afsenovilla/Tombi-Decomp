// FUNC 80068afc 12 MAIN0
// MATCHING 80068afc 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl ReturnFromException\n.type ReturnFromException,@function\nReturnFromException:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,23\n"
".set at\n.set reorder\n");
