// VERSION SLPS_01144 slps01144 80059D8C 48
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPS_01144)
__asm__(".set noreorder\n.set noat\n"
".globl func_80059D8C\n.type func_80059D8C,@function\nfunc_80059D8C:\n"
"	lui	$8,%hi(func_80059D8C)\n"
"	addiu	$8,$8,%lo(func_80059D8C)\n"
"	jalr	$8\n"
"	sll	$0,$0,0x0\n"
"	sll	$0,$0,0x0\n"
"	sll	$0,$0,0x0\n"
"	addiu	$8,$0,200\n"
"Lfunc_80059D8C_1c:\n"
"	addiu	$8,$8,-1\n"
"	bne	$8,$0,Lfunc_80059D8C_1c\n"
"	sll	$0,$0,0x0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
