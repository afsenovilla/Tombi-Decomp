// VERSION PCPX_96084 tomba 800950B0 16
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_PCPX_96084)
// SN host-file call, break 0x104 (PCcreat).
__asm__(".set noreorder\n.set noat\n"
".globl func_800950B0_pcpx96084\n.type func_800950B0_pcpx96084,@function\nfunc_800950B0_pcpx96084:\n"
"\tmove	$5,$4\n"
"\tbreak	0,0x104\n"
"\tjr	$31\n"
"\tnop\n"
".set at\n.set reorder\n");
#endif
