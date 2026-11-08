// VERSION SLPS_01144 slps01144 80064F04 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl SysEnqIntRP\n.type SysEnqIntRP,@function\nSysEnqIntRP:\n"
"	addiu	$10,$0,192\n"
"	jr	$10\n"
"	addiu	$9,$0,2\n"
".set at\n.set reorder\n");
#endif
