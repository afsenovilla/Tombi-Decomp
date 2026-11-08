// VERSION BETA_980203 beta980203 800621B8 40
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/LightColor_slps01144.c (LightColor) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl LightColor\n.type LightColor,@function\nLightColor:\n"
"	lwc2	$9,0($4)\n"
"	lwc2	$10,4($4)\n"
"	lwc2	$11,8($4)\n"
"	sll	$0,$0,0x0\n"
"	c2	0x4da412\n"
"	swc2	$9,0($5)\n"
"	swc2	$10,4($5)\n"
"	swc2	$11,8($5)\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
