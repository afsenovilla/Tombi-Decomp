// VERSION BETA_980203 beta980203 80061FB8 48
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/SetLightMatrix_slps01144.c (SetLightMatrix) [-O2 -G0] (tools/demo_sweep.py)
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
