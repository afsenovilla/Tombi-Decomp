// VERSION SLPM_80165 slpm80165 8005A104 44
// AS.SLPM_80165 gnu
#if defined(VERSION_SLPM_80165)
// Demo version: SLPS_01144 C of src/func_800614F8_slps01144.c (func_800614F8, twin score 0.932) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl func_800614F8\n.type func_800614F8,@function\nfunc_800614F8:\n"
"	addiu	$26,$0,256\n"
"	lw	$26,8($26)\n"
"	sll	$0,$0,0x0\n"
"	lw	$26,0($26)\n"
"	sll	$0,$0,0x0\n"
"	addi	$26,$26,8\n"
"	sw	$1,4($26)\n"
"	sw	$2,8($26)\n"
"	sw	$3,12($26)\n"
"	sw	$31,124($26)\n"
"	mfc0	$2,$13\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
