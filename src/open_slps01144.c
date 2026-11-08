// VERSION SLPS_01144 slps01144 800584EC 44
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl open_slps01144\n.type open_slps01144,@function\nopen_slps01144:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,50\n"
"	sll	$0,$0,0x0\n"
".globl lseek\n.type lseek,@function\nlseek:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,51\n"
"	sll	$0,$0,0x0\n"
".globl read\n.type read,@function\nread:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,52\n"
".set at\n.set reorder\n");
#endif
