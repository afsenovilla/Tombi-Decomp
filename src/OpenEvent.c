// FUNC 8005b954 12 MAIN0
// MATCHING 8005b954 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl OpenEvent\n.type OpenEvent,@function\nOpenEvent:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,8\n"
".set at\n.set reorder\n");
