// VERSION SCUS_94236 scus94236 8005B48C 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl format\n.type format,@function\nformat:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,65\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
