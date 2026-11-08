// VERSION SLPS_01144 slps01144 80065198 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl ChangeClearRCnt\n.type ChangeClearRCnt,@function\nChangeClearRCnt:\n"
"	addiu	$10,$0,192\n"
"	jr	$10\n"
"	addiu	$9,$0,10\n"
".set at\n.set reorder\n");
#endif
