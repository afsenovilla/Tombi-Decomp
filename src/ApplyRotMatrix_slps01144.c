// VERSION SLPS_01144 slps01144 80060518 48
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl ApplyRotMatrix\n.type ApplyRotMatrix,@function\nApplyRotMatrix:\n"
"	lw	$8,0($4)\n"
"	lw	$9,4($4)\n"
"	mtc2	$8,$0\n"
"	mtc2	$9,$1\n"
"	sll	$0,$0,0x0\n"
"	c2	0x486012\n"
"	swc2	$9,0($5)\n"
"	swc2	$10,4($5)\n"
"	swc2	$11,8($5)\n"
"	addu	$2,$6,$0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
