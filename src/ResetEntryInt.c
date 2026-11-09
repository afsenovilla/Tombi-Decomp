// FUNC 80068b0c 12 MAIN0
// MATCHING 80068b0c 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl ResetEntryInt\n.type ResetEntryInt,@function\nResetEntryInt:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,24\n"
".set at\n.set reorder\n");
