// FUNC 80063e3c 48 MAIN0
// MATCHING 80063e3c 48
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl SetColorMatrix\n.type SetColorMatrix,@function\nSetColorMatrix:\n"
"	lw	$8,0($4)\n"
"	lw	$9,4($4)\n"
"	lw	$10,8($4)\n"
"	lw	$11,12($4)\n"
"	lw	$12,16($4)\n"
"	ctc2	$8,$16\n"
"	ctc2	$9,$17\n"
"	ctc2	$10,$18\n"
"	ctc2	$11,$19\n"
"	ctc2	$12,$20\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
