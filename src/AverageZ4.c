// FUNC 8006412c 36 MAIN0
// MATCHING 8006412c 36
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl AverageZ4\n.type AverageZ4,@function\nAverageZ4:\n"
"	mtc2	$4,$16\n"
"	mtc2	$5,$17\n"
"	mtc2	$6,$18\n"
"	mtc2	$7,$19\n"
"	sll	$0,$0,0x0\n"
"	c2	0x168002e\n"
"	mfc2	$2,$7\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
