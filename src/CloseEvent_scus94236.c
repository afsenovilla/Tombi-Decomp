// VERSION SCUS_94236 scus94236 8005B3BC 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl CloseEvent\n.type CloseEvent,@function\nCloseEvent:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,9\n"
".set at\n.set reorder\n");
#endif
