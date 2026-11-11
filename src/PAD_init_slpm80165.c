// VERSION SLPM_80165 slpm80165 8005DB00 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo-only: BIOS B0 call 15h (PAD_init); PAL drives the pads through libpad instead.
__asm__(".set noreorder\n.set noat\n"
".globl PAD_init\n.type PAD_init,@function\nPAD_init:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,21\n"
".set at\n.set reorder\n");
#endif
