// VERSION PCPX_96084 tomba 800951B0 24
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_PCPX_96084)
// SN host-file call, break 0x105 (arguments already in a1..a3, called by the chunked transfer at 800950F0); -1 on failure.
__asm__(".set noreorder\n.set noat\n"
".globl func_800951B0_pcpx96084\n.type func_800951B0_pcpx96084,@function\nfunc_800951B0_pcpx96084:\n"
"\tbreak	0,0x105\n"
"\tbeqz	$2,1f\n"
"\tmove	$2,$3\n"
"\tli	$2,-1\n"
"1:\tjr	$31\n"
"\tnop\n"
".set at\n.set reorder\n");
#endif
