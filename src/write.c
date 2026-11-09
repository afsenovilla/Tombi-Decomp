// FUNC 8005ba14 12 MAIN0
// MATCHING 8005ba14 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl write\n.type write,@function\nwrite:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,53\n"
".set at\n.set reorder\n");
