// VERSION SLPS_01144 slps01144 8006050C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl func_8006050C\n.type func_8006050C,@function\nfunc_8006050C:\n"
"	sw	$10,8($6)\n"
"	jr	$31\n"
"	addu	$2,$6,$0\n"
".set at\n.set reorder\n");
#endif
