// FUNC 80064d14 12 MAIN0
// MATCHING 80064d14 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl DeliverEvent\n.type DeliverEvent,@function\nDeliverEvent:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,7\n"
".set at\n.set reorder\n");
