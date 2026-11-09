// VERSION PCPX_96084 tomba 80093ECC 12
// AS.PCPX_96084 gnu
#if defined(VERSION_PCPX_96084)
/* Psy-Q BIOS B0 0x16 trampoline: `jr $t2` with the call number in the delay slot (like src/OpenEvent.c). */
__asm__(".set noreorder\n.set noat\n"
".globl func_80093ECC_pcpx96084\n.type func_80093ECC_pcpx96084,@function\nfunc_80093ECC_pcpx96084:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,22\n"
".set at\n.set reorder\n");
#endif
