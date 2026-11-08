// VERSION SCUS_94236 scus94236 8005B39C 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl _bu_init\n.type _bu_init,@function\n_bu_init:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,112\n"
".set at\n.set reorder\n");
#endif
