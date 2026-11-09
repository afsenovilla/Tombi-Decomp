// FUNC 80068b1c 12 MAIN0
// MATCHING 80068b1c 12
// FLAGS -O2 -G0
// AS gnu
__asm__(".set noreorder\n.set noat\n"
".globl HookEntryInt\n.type HookEntryInt,@function\nHookEntryInt:\n"
"	addiu	$10,$0,176\n"
"	jr	$10\n"
"	addiu	$9,$0,25\n"
".set at\n.set reorder\n");
