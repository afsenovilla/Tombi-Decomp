// VERSION PCPX_96084 tomba 80094234 20
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_PCPX_96084)
// Indirect call through the first word of the callback table at 0x800C80B8 (libetc callback trampoline).
__asm__(".set noreorder\n.set noat\n"
".globl func_80094234_pcpx96084\n.type func_80094234_pcpx96084,@function\nfunc_80094234_pcpx96084:\n"
"\tlui	$9,%hi(jtbl_800C80B8)\n"
"\tlw	$9,%lo(jtbl_800C80B8)($9)\n"
"\tnop\n"
"\tjr	$9\n"
"\tnop\n"
".set at\n.set reorder\n");
#endif
