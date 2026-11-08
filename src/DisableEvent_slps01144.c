// VERSION SLPS_01144 slps01144 8006F488 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl DisableEvent\n.type DisableEvent,@function\nDisableEvent:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,13\n"
".set at\n.set reorder\n");
#endif
