// FUNC 8005b9c4 16 MAIN0
// MATCHING 8005b9c4 16
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl EnterCriticalSection\n.type EnterCriticalSection,@function\nEnterCriticalSection:\n"
"	addiu	$4,$0,1\n"
"	syscall	\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
