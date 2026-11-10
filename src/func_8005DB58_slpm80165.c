// VERSION SLPM_80165 slpm80165 8005DB58 104
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo-only Psy-Q card patch (B0 57h table, like PAL _patch_card2): stores two entry addresses of the BIOS table in
// jtbl_80088F48/jtbl_80088F4C (read by func_8005DB30/func_8005DB44) and clears 11 words at +0x594.
__asm__(".set noreorder\n.set noat\n"
".globl func_8005DB58\n.type func_8005DB58,@function\nfunc_8005DB58:\n"
"	lui	$1,%hi(D_80088F40)\n"
"	sw	$31,%lo(D_80088F40)($1)\n"
"	jal	EnterCriticalSection\n"
"	sll	$0,$0,0x0\n"
"	addiu	$10,$0,176\n"
"	jalr	$10\n"
"	addiu	$9,$0,87\n"
"	lw	$2,364($2)\n"
"	lui	$1,%hi(jtbl_80088F48)\n"
"	addi	$3,$2,2180\n"
"	sw	$3,%lo(jtbl_80088F48)($1)\n"
"	lui	$1,%hi(jtbl_80088F4C)\n"
"	addi	$3,$2,2196\n"
"	addiu	$9,$0,11\n"
"	sw	$3,%lo(jtbl_80088F4C)($1)\n"
"Lfunc_8005DB58_44:\n"
"	addiu	$9,$9,-1\n"
"	sw	$0,1428($2)\n"
"	bnez	$9,Lfunc_8005DB58_44\n"
"	addiu	$2,$2,4\n"
"	jal	FlushCache\n"
"	sll	$0,$0,0x0\n"
"	lui	$31,%hi(D_80088F40)\n"
"	lw	$31,%lo(D_80088F40)($31)\n"
"	sll	$0,$0,0x0\n"
"	jr	$31\n"
"	sll	$0,$0,0x0\n"
".set at\n.set reorder\n");
#endif
