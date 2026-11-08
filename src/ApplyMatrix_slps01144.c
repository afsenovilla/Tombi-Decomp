// VERSION SLPS_01144 slps01144 80060548 80
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl ApplyMatrix\n.type ApplyMatrix,@function\nApplyMatrix:\n"
"	lw	$8,0($4)\n"
"	lw	$9,4($4)\n"
"	lw	$10,8($4)\n"
"	lw	$11,12($4)\n"
"	lw	$12,16($4)\n"
"	ctc2	$8,$0\n"
"	ctc2	$9,$1\n"
"	ctc2	$10,$2\n"
"	ctc2	$11,$3\n"
"	ctc2	$12,$4\n"
"	lwc2	$0,0($5)\n"
"	lwc2	$1,4($5)\n"
"	sll	$0,$0,0x0\n"
"	c2	0x486012\n"
"	swc2	$25,0($6)\n"
"	swc2	$26,4($6)\n"
"	swc2	$27,8($6)\n"
"	addu	$2,$6,$0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
