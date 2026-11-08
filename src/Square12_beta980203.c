// VERSION BETA_980203 beta980203 80062268 40
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/Square12_slps01144.c (Square12) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl Square12\n.type Square12,@function\nSquare12:\n"
"	lwc2	$9,0($4)\n"
"	lwc2	$10,4($4)\n"
"	lwc2	$11,8($4)\n"
"	sll	$0,$0,0x0\n"
"	c2	0xa80428\n"
"	swc2	$25,0($5)\n"
"	swc2	$26,4($5)\n"
"	swc2	$27,8($5)\n"
"	jr	$31\n"
"	addu	$2,$5,$0\n"
".set at\n.set reorder\n");
#endif
