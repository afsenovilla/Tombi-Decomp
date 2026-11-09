// FUNC 80063ddc 48 MAIN0
// MATCHING 80063ddc 48
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl SetRotMatrix\n.type SetRotMatrix,@function\nSetRotMatrix:\n"
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
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
