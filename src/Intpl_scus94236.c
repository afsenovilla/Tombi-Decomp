// VERSION SCUS_94236 scus94236 80063AF0 36
// AS.SCUS_94236 gnu
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/Intpl_slps01144.c (Intpl) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl Intpl\n.type Intpl,@function\nIntpl:\n"
"	lwc2	$9,0($4)\n"
"	lwc2	$10,4($4)\n"
"	lwc2	$11,8($4)\n"
"	mtc2	$5,$8\n"
"	sll	$0,$0,0x0\n"
"	c2	0x980011\n"
"	swc2	$22,0($6)\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
