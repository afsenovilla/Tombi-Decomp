// FUNC 8007594c 12 MAIN0
// MATCHING 8007594c 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl DisableEvent\n.type DisableEvent,@function\nDisableEvent:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,13\n"
".set at\n.set reorder\n");
