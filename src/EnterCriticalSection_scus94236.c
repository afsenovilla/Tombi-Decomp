// VERSION SCUS_94236 scus94236 8005B41C 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl EnterCriticalSection\n.type EnterCriticalSection,@function\nEnterCriticalSection:\n"
"	addiu	$4,$0,1\n"
"	syscall	\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
