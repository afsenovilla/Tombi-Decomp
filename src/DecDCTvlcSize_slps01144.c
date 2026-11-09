// VERSION SLPS_01144 slps01144 8005A640 48
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl DecDCTvlcSize\n.type DecDCTvlcSize,@function\nDecDCTvlcSize:\n"
"	lui	$8,%hi(func_8005A63C)\n"
"	addiu	$8,$8,%lo(func_8005A63C)\n"
"	addi	$1,$4,-1\n"
"	blez	$1,LDecDCTvlcSize_20\n"
"	lw	$2,0($8)\n"
"	sll	$1,$4,0x1\n"
"	jr	$31\n"
"	sw	$1,0($8)\n"
"LDecDCTvlcSize_20:\n"
"	lui	$1,0xff\n"
"	ori	$1,$1,0xffff\n"
"	jr	$31\n"
"	sw	$1,0($8)\n"
".set at\n.set reorder\n");
#endif
