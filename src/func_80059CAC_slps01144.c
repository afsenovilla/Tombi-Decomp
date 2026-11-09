// VERSION SLPS_01144 slps01144 80059CAC 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl func_80059CAC\n.type func_80059CAC,@function\nfunc_80059CAC:\n"
"	lui	$2,%hi(PATCH_OBJ_14)\n"
"	addiu	$2,$2,%lo(PATCH_OBJ_14)\n"
"	jr	$2\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
