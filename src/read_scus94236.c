// VERSION SCUS_94236 scus94236 8005B45C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl read\n.type read,@function\nread:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,52\n"
".set at\n.set reorder\n");
#endif
