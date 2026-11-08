// VERSION PCPX_96084 tomba 80094C18 60
// AS.PCPX_96084 gnu
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/setjmp_slps01144.c (setjmp) [-O2 -G0] (tools/demo_sweep.py)
__asm__(".set noreorder\n.set noat\n"
".globl setjmp\n.type setjmp,@function\nsetjmp:\n"
"	sw	$31,0($4)\n"
"	sw	$28,44($4)\n"
"	sw	$29,4($4)\n"
"	sw	$30,8($4)\n"
"	sw	$16,12($4)\n"
"	sw	$17,16($4)\n"
"	sw	$18,20($4)\n"
"	sw	$19,24($4)\n"
"	sw	$20,28($4)\n"
"	sw	$21,32($4)\n"
"	sw	$22,36($4)\n"
"	sw	$23,40($4)\n"
"	addu	$2,$0,$0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
