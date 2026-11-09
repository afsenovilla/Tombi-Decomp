// FUNC 8006421c 44 MAIN0
// MATCHING 8006421c 44
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl RotTransPers\n.type RotTransPers,@function\nRotTransPers:\n"
"	lwc2	$0,0($4)\n"
"	lwc2	$1,4($4)\n"
"	sll	$0,$0,0x0\n"
"	c2	0x180001\n"
"	swc2	$14,0($5)\n"
"	swc2	$8,0($6)\n"
"	cfc2	$3,$31\n"
"	mfc2	$2,$19\n"
"	sw	$3,0($7)\n"
"	jr	$31\n"
"	sra	$2,$2,0x2\n"
".set at\n.set reorder\n");
