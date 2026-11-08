// VERSION SCUS_94236 scus94236 80063834 48
// AS.SCUS_94236 gnu
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/SetRotMatrix_slps01144.c (SetRotMatrix) [-O2 -G0] (tools/demo_sweep.py)
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
#endif
