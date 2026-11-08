// VERSION SLPS_01144 slps01144 800584DC 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl ExitCriticalSection\n.type ExitCriticalSection,@function\nExitCriticalSection:\n"
"	addiu	$4,$0,2\n"
"	syscall	\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
