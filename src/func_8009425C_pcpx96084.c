// VERSION PCPX_96084 tomba 8009425C 104
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_PCPX_96084)
// PrePre 8 twin of the JP patch routine src/func_80064F4C_slps01144.c (other data addresses).
__asm__(".set noreorder\n.set noat\n"
".globl func_8009425C_pcpx96084\n.type func_8009425C_pcpx96084,@function\nfunc_8009425C_pcpx96084:\n"
"	lui	$1,%hi(D_800C80B0)\n"
"	sw	$31,%lo(D_800C80B0)($1)\n"
"	jal	EnterCriticalSection\n"
"	sll	$0,$0,0x0\n"
"	addiu	$10,$0,176\n"
"	jalr	$10\n"
"	addiu	$9,$0,87\n"
"	lw	$2,364($2)\n"
"	lui	$1,%hi(jtbl_800C80B8)\n"
"	addi	$3,$2,2180\n"
"	sw	$3,%lo(jtbl_800C80B8)($1)\n"
"	lui	$1,%hi(jtbl_800C80BC)\n"
"	addi	$3,$2,2196\n"
"	addiu	$9,$0,11\n"
"	sw	$3,%lo(jtbl_800C80BC)($1)\n"
"Lfunc_8009425C_pcpx96084_3c:\n"
"	addiu	$9,$9,-1\n"
"	sw	$0,1428($2)\n"
"	bne	$9,$0,Lfunc_8009425C_pcpx96084_3c\n"
"	addiu	$2,$2,4\n"
"	jal	FlushCache\n"
"	sll	$0,$0,0x0\n"
"	lui	$31,%hi(D_800C80B0)\n"
"	lw	$31,%lo(D_800C80B0)($31)\n"
"	sll	$0,$0,0x0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
