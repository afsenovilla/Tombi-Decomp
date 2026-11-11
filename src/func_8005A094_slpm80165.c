// VERSION SLPM_80165 slpm80165 8005A094 104
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo version of PAL psyq_8005d510 (src/psyq_8005d510.c): copies the words from D_8005A0FC up to StSetRing, stored
// one word lower.
__asm__(".set noreorder\n.set noat\n"
".globl func_8005A094\n.type func_8005A094,@function\nfunc_8005A094:\n"
"	lui	$1,%hi(D_80088F30)\n"
"	sw	$31,%lo(D_80088F30)($1)\n"
"	jal	EnterCriticalSection\n"
"	sll	$0,$0,0x0\n"
"	addiu	$10,$0,176\n"
"	jalr	$10\n"
"	addiu	$9,$0,86\n"
"	lui	$10,%hi(D_8005A0FC)\n"
"	lui	$9,%hi(StSetRing)\n"
"	lw	$2,24($2)\n"
"	addiu	$10,$10,%lo(D_8005A0FC)\n"
"	addiu	$9,$9,%lo(StSetRing)\n"
"Lfunc_8005A094_30:\n"
"	lw	$3,0($10)\n"
"	addiu	$10,$10,4\n"
"	addiu	$2,$2,4\n"
"	bne	$10,$9,Lfunc_8005A094_30\n"
"	sw	$3,-4($2)\n"
"	jal	FlushCache\n"
"	sll	$0,$0,0x0\n"
"	jal	ExitCriticalSection\n"
"	sll	$0,$0,0x0\n"
"	lui	$31,%hi(D_80088F30)\n"
"	lw	$31,%lo(D_80088F30)($31)\n"
"	sll	$0,$0,0x0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
