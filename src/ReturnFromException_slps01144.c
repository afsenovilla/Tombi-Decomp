// VERSION SLPS_01144 slps01144 800658D8 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl ReturnFromException\n.type ReturnFromException,@function\nReturnFromException:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,23\n"
".set at\n.set reorder\n");
#endif
