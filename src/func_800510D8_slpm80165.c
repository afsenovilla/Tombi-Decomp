// VERSION SLPM_80165 slpm80165 800510D8 16
// AS.SLPM_80165 gnu
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/EnterCriticalSection_slps01144.c (EnterCriticalSection) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl func_800510D8_slpm80165\n.type func_800510D8_slpm80165,@function\nEnterCriticalSection:\n"
"	addiu	$4,$0,1\n"
"	syscall	\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
