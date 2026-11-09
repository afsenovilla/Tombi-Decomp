// FUNC 800683bc 12 MAIN0
// MATCHING 800683bc 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl ChangeClearRCnt\n.type ChangeClearRCnt,@function\nChangeClearRCnt:\n"
"	addiu	$10,$0,192\n"
"	jr	$10\n"
"	addiu	$9,$0,10\n"
".set at\n.set reorder\n");
