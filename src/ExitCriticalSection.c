// FUNC 8005b9d4 16 MAIN0
// MATCHING 8005b9d4 16
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl ExitCriticalSection\n.type ExitCriticalSection,@function\nExitCriticalSection:\n"
"	addiu	$4,$0,2\n"
"	syscall	\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
