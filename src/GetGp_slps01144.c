// VERSION SLPS_01144 slps01144 8005857C 8
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl GetGp\n.type GetGp,@function\nGetGp:\n"
"	jr	$31\n"
"	addu	$2,$28,$0\n"
".set at\n.set reorder\n");
#endif
