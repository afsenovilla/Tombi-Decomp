// VERSION SLPS_01144 slps01144 80064F38 20
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl func_80064F38\n.type func_80064F38,@function\nfunc_80064F38:\n"
"	lui	$9,%hi(jtbl_8009DB04)\n"
"	lw	$9,%lo(jtbl_8009DB04)($9)\n"
"	sll	$0,$0,0x0\n"
"	jr	$9\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
