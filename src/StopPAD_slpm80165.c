// VERSION SLPM_80165 slpm80165 8005DAF0 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_SLPM_80165)
// Demo-only: BIOS B0 call 14h (StopPAD); PAL drives the pads through libpad instead.
__asm__(".set noreorder\n.set noat\n"
".globl StopPAD\n.type StopPAD,@function\nStopPAD:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,20\n"
".set at\n.set reorder\n");
#endif
