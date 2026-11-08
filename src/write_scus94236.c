// VERSION SCUS_94236 scus94236 8005B46C 76
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl write\n.type write,@function\nwrite:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,53\n"
"	sll	$0,$0,0x0\n"
".globl close\n.type close,@function\nclose:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,54\n"
"	sll	$0,$0,0x0\n"
".globl format\n.type format,@function\nformat:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,65\n"
"	sll	$0,$0,0x0\n"
".globl nextfile\n.type nextfile,@function\nnextfile:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,67\n"
"	sll	$0,$0,0x0\n"
".globl erase\n.type erase,@function\nerase:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,69\n"
".set at\n.set reorder\n");
#endif
