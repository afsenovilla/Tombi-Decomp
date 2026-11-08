// VERSION SCUS_94236 scus94236 8005B4BC 8
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl GetGp\n.type GetGp,@function\nGetGp:\n"
"	jr	$31\n"
"	addu	$2,$28,$0\n"
".set at\n.set reorder\n");
#endif
