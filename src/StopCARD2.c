// FUNC 8005d360 12 MAIN0
// MATCHING 8005d360 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl StopCARD2\n.type StopCARD2,@function\nStopCARD2:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,76\n"
".set at\n.set reorder\n");
