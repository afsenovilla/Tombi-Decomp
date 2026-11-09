// VERSION SLPS_01144 slps01144 80061660 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl func_80061660\n.type func_80061660,@function\nfunc_80061660:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,7\n"
".set at\n.set reorder\n");
#endif
