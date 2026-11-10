// VERSION PCPX_96084 tomba 800950E4 12
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_PCPX_96084)
// SN host-file call, break 0x101 (PCinit).
__asm__(".set noreorder\n.set noat\n"
".globl func_800950E4_pcpx96084\n.type func_800950E4_pcpx96084,@function\nfunc_800950E4_pcpx96084:\n"
"\tbreak	0,0x101\n"
"\tjr	$31\n"
"\tnop\n"
".set at\n.set reorder\n");
#endif
