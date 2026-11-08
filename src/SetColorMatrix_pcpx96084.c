// VERSION PCPX_96084 tomba 8008F3C4 48
// AS.PCPX_96084 gnu
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/SetColorMatrix_slps01144.c (SetColorMatrix) [-O2 -G0] (tools/demo_sweep.py)
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
#endif
