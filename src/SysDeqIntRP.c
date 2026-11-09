// FUNC 8006a244 12 MAIN0
// MATCHING 8006a244 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl SysDeqIntRP\n.type SysDeqIntRP,@function\nSysDeqIntRP:\n"
"	addiu	$10,$0,192\n"
"	jr	$10\n"
"	addiu	$9,$0,3\n"
".set at\n.set reorder\n");
