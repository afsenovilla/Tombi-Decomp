// VERSION SLPS_01144 slps01144 80060598 92
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl ApplyMatrixSV\n.type ApplyMatrixSV,@function\nApplyMatrixSV:\n"
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
"	mfc2	$8,$9\n"
"	mfc2	$9,$10\n"
"	mfc2	$10,$11\n"
"	sh	$8,0($6)\n"
"	sh	$9,2($6)\n"
"	sh	$10,4($6)\n"
"	addu	$2,$6,$0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
