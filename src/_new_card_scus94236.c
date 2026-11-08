// VERSION SCUS_94236 scus94236 8005CCBC 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl _new_card\n.type _new_card,@function\n_new_card:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,80\n"
".set at\n.set reorder\n");
#endif
