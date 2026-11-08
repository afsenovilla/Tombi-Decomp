// VERSION SLPS_01144 slps01144 80059E4C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl FlushCache\n.type FlushCache,@function\nFlushCache:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,68\n"
".set at\n.set reorder\n");
#endif
