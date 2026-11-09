// VERSION SLPS_01144 slps01144 8005852C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl close\n.type close,@function\nclose:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,54\n"
".set at\n.set reorder\n");
#endif
