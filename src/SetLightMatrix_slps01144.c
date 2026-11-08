// VERSION SLPS_01144 slps01144 80060758 48
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl SetLightMatrix\n.type SetLightMatrix,@function\nSetLightMatrix:\n"
"	lw	$8,0($4)\n"
"	lw	$9,4($4)\n"
"	lw	$10,8($4)\n"
"	lw	$11,12($4)\n"
"	lw	$12,16($4)\n"
"	ctc2	$8,$8\n"
"	ctc2	$9,$9\n"
"	ctc2	$10,$10\n"
"	ctc2	$11,$11\n"
"	ctc2	$12,$12\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
