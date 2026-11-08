// VERSION SCUS_94236 scus94236 800685C0 60
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl longjmp\n.type longjmp,@function\nlongjmp:\n"
"	lw	$31,0($4)\n"
"	lw	$28,44($4)\n"
"	lw	$29,4($4)\n"
"	lw	$30,8($4)\n"
"	lw	$16,12($4)\n"
"	lw	$17,16($4)\n"
"	lw	$18,20($4)\n"
"	lw	$19,24($4)\n"
"	lw	$20,28($4)\n"
"	lw	$21,32($4)\n"
"	lw	$22,36($4)\n"
"	lw	$23,40($4)\n"
"	addu	$2,$5,$0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
