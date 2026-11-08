// VERSION SLPM_80165 slpm80165 800510E8 16
// AS.SLPM_80165 gnu
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/ExitCriticalSection_slps01144.c (ExitCriticalSection) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl func_800510E8_slpm80165\n.type func_800510E8_slpm80165,@function\nExitCriticalSection:\n"
"	addiu	$4,$0,2\n"
"	syscall	\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
