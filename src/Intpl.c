// FUNC 80064098 36 MAIN0
// MATCHING 80064098 36
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl Intpl\n.type Intpl,@function\nIntpl:\n"
"	lwc2	$9,0($4)\n"
"	lwc2	$10,4($4)\n"
"	lwc2	$11,8($4)\n"
"	mtc2	$5,$8\n"
"	sll	$0,$0,0x0\n"
"	c2	0x980011\n"
"	swc2	$22,0($6)\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
