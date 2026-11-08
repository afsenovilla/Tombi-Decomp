// VERSION SLPS_01144 slps01144 8005844C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl _bu_init\n.type _bu_init,@function\n_bu_init:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,112\n"
".set at\n.set reorder\n");
#endif
