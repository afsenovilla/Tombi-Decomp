// VERSION SCUS_94236 scus94236 8006B70C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl InitHeap\n.type InitHeap,@function\nInitHeap:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,57\n"
".set at\n.set reorder\n");
#endif
