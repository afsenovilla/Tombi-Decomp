// VERSION SCUS_94236 scus94236 80063C00 88
// AS.SCUS_94236 gnu
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/OuterProduct0_slps01144.c (OuterProduct0) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl OuterProduct0\n.type OuterProduct0,@function\nOuterProduct0:\n"
"	cfc2	$13,$0\n"
"	cfc2	$14,$2\n"
"	cfc2	$15,$4\n"
"	lw	$8,0($4)\n"
"	lw	$9,4($4)\n"
"	lw	$10,8($4)\n"
"	ctc2	$8,$0\n"
"	ctc2	$9,$2\n"
"	ctc2	$10,$4\n"
"	lwc2	$11,8($5)\n"
"	lwc2	$9,0($5)\n"
"	lwc2	$10,4($5)\n"
"	sll	$0,$0,0x0\n"
"	c2	0x170000c\n"
"	swc2	$25,0($6)\n"
"	swc2	$26,4($6)\n"
"	swc2	$27,8($6)\n"
"	ctc2	$13,$0\n"
"	ctc2	$14,$2\n"
"	ctc2	$15,$4\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
