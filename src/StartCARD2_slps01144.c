// VERSION SLPS_01144 slps01144 80059C8C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl StartCARD2\n.type StartCARD2,@function\nStartCARD2:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,75\n"
".set at\n.set reorder\n");
#endif
