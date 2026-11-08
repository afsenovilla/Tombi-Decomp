// VERSION SLPS_01144 slps01144 800609A8 60
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl DpqColor3\n.type DpqColor3,@function\nDpqColor3:\n"
"	lwc2	$20,0($4)\n"
"	lwc2	$21,0($5)\n"
"	lwc2	$22,0($6)\n"
"	lwc2	$6,0($6)\n"
"	mtc2	$7,$8\n"
"	sll	$0,$0,0x0\n"
"	c2	0xf8002a\n"
"	lw	$8,16($29)\n"
"	lw	$9,20($29)\n"
"	lw	$10,24($29)\n"
"	swc2	$20,0($8)\n"
"	swc2	$21,0($9)\n"
"	swc2	$22,0($10)\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
