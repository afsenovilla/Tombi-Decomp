// VERSION SLPM_80165 slpm80165 8005D7C8 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo-only: BIOS B0 call 16h (PAD_dr); PAL drives the pads through libpad instead.
__asm__(".set noreorder\n.set noat\n"
".globl PAD_dr\n.type PAD_dr,@function\nPAD_dr:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,22\n"
".set at\n.set reorder\n");
#endif
