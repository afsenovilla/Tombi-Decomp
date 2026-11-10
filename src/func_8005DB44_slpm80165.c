// VERSION SLPM_80165 slpm80165 8005DB44 20
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo version of the JP func_80064F24 / func_80064F38 trampolines (jump through a pointer word).
__asm__(".set noreorder\n.set noat\n"
".globl func_8005DB44\n.type func_8005DB44,@function\nfunc_8005DB44:\n"
"	lui	$9,%hi(jtbl_80088F4C)\n"
"	lw	$9,%lo(jtbl_80088F4C)($9)\n"
"	sll	$0,$0,0x0\n"
"	jr	$9\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
