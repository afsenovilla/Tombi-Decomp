// VERSION SLPS_01144 slps01144 800609E4 36
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
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
