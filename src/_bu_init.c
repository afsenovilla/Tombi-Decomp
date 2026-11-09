// FUNC 8005b944 12 MAIN0
// MATCHING 8005b944 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl _bu_init\n.type _bu_init,@function\n_bu_init:\n"
"	addiu	$10,$0,160\n"
"	jr	$10\n"
"	addiu	$9,$0,112\n"
".set at\n.set reorder\n");
