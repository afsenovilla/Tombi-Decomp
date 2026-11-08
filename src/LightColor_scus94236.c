// VERSION SCUS_94236 scus94236 80063A64 40
// AS.SCUS_94236 gnu
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/LightColor_slps01144.c (LightColor) [-O2 -G0] (tools/demo_sweep.py)
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
