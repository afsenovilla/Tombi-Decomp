// VERSION PCPX_96084 tomba 8008746C 12
// AS.PCPX_96084 gnu
#if defined(VERSION_PCPX_96084)
/* Psy-Q BIOS A0 0xA0 trampoline: `jr $t2` with the call number in the delay slot (like src/OpenEvent.c). */
__asm__(".set noreorder\n.set noat\n"
".globl func_8008746C_pcpx96084\n.type func_8008746C_pcpx96084,@function\nfunc_8008746C_pcpx96084:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,160\n"
".set at\n.set reorder\n");
#endif
