// FUNC 8006c1c4 12 MAIN0
// MATCHING 8006c1c4 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl InitHeap\n.type InitHeap,@function\nInitHeap:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,57\n"
".set at\n.set reorder\n");
