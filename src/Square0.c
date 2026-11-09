// FUNC 800640e4 40 MAIN0
// MATCHING 800640e4 40
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl Square0\n.type Square0,@function\nSquare0:\n"
"	lwc2	$9,0($4)\n"
"	lwc2	$10,4($4)\n"
"	lwc2	$11,8($4)\n"
"	sll	$0,$0,0x0\n"
"	c2	0xa00428\n"
"	swc2	$25,0($5)\n"
"	swc2	$26,4($5)\n"
"	swc2	$27,8($5)\n"
"	jr	$31\n"
"	addu	$2,$5,$0\n"
".set at\n.set reorder\n");
