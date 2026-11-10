// VERSION SLPM_80165 slpm80165 800528B8 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo version of the JP func_80059CAC (src/func_80059CAC_slps01144.c): jumps to PATCH_OBJ_14.
__asm__(".set noreorder\n.set noat\n"
".globl func_800528B8_slpm80165\n.type func_800528B8_slpm80165,@function\nfunc_800528B8_slpm80165:\n"
"	lui	$2,%hi(PATCH_OBJ_14)\n"
"	addiu	$2,$2,%lo(PATCH_OBJ_14)\n"
"	jr	$2\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
