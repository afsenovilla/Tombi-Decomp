// VERSION PCPX_96084 tomba 80095090 32
// FLAGS -O2 -G0
// AS gnu
#if defined(VERSION_PCPX_96084)
// SN host-file call, break 0x103 (PCopen: name, flags, mode); -1 on failure.
__asm__(".set noreorder\n.set noat\n"
".globl func_80095090_pcpx96084\n.type func_80095090_pcpx96084,@function\nfunc_80095090_pcpx96084:\n"
"\tmove	$6,$5\n"
"\tmove	$5,$4\n"
"\tbreak	0,0x103\n"
"\tbeqz	$2,1f\n"
"\tmove	$2,$3\n"
"\tli	$2,-1\n"
"1:\tjr	$31\n"
"\tnop\n"
".set at\n.set reorder\n");
#endif
