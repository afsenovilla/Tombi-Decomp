// VERSION SCUS_94236 scus94236 80063A8C 40
// AS.SCUS_94236 gnu
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/DpqColorLight_slps01144.c (DpqColorLight) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl DpqColorLight\n.type DpqColorLight,@function\nDpqColorLight:\n"
"	lwc2	$9,0($4)\n"
"	lwc2	$10,4($4)\n"
"	lwc2	$11,8($4)\n"
"	lwc2	$6,0($5)\n"
"	mtc2	$6,$8\n"
"	sll	$0,$0,0x0\n"
"	c2	0x680029\n"
"	swc2	$22,0($7)\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
