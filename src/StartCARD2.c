// FUNC 8005d350 12 MAIN0
// MATCHING 8005d350 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl StartCARD2\n.type StartCARD2,@function\nStartCARD2:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,75\n"
".set at\n.set reorder\n");
