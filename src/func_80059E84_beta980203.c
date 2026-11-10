// VERSION BETA_980203 beta980203 80059E84 44
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/open_slps01144.c (open, lseek, read: BIOS B0 calls 0x32..0x34, one function in the symbol table, as in JP).
__asm__(".set noreorder\n.set noat\n"
".globl func_80059E84_beta980203\n.type func_80059E84_beta980203,@function\nfunc_80059E84_beta980203:\n"
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
