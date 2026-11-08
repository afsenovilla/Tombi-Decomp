// VERSION SCUS_94236 scus94236 80063A54 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SCUS_94236)
__asm__(".set noreorder\n.set noat\n"
".globl SetGeomScreen\n.type SetGeomScreen,@function\nSetGeomScreen:\n"
"	ctc2	$4,$26\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
