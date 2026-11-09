// FUNC 8006410c 32 MAIN0
// MATCHING 8006410c 32
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl AverageZ3\n.type AverageZ3,@function\nAverageZ3:\n"
"	mtc2	$4,$17\n"
"	mtc2	$5,$18\n"
"	mtc2	$6,$19\n"
"	sll	$0,$0,0x0\n"
"	c2	0x158002d\n"
"	mfc2	$2,$7\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
