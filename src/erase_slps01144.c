// VERSION SLPS_01144 slps01144 8005856C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl erase\n.type erase,@function\nerase:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,69\n"
".set at\n.set reorder\n");
#endif
