// FUNC 80061cd0 12 MAIN0
// MATCHING 80061cd0 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl GPU_cw\n.type GPU_cw,@function\nGPU_cw:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,73\n"
".set at\n.set reorder\n");
