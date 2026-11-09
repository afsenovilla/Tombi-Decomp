// FUNC 8005ba64 8 MAIN0
// MATCHING 8005ba64 8
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl GetGp\n.type GetGp,@function\nGetGp:\n"
"	jr	$31\n"
"	addu	$2,$28,$0\n"
".set at\n.set reorder\n");
