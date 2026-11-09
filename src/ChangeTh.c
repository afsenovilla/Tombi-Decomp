// FUNC 8005b9b4 12 MAIN0
// MATCHING 8005b9b4 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl ChangeTh\n.type ChangeTh,@function\nChangeTh:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,16\n"
".set at\n.set reorder\n");
