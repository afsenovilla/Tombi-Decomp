// FUNC 80068ae4 12 MAIN0
// MATCHING 80068ae4 12
/* Psy-Q libapi BIOS A0 trampoline (call 0x72): `jr $t2` with the call number in the delay slot. Top-level asm so
   cc1 emits no prologue/epilogue; the old in-function asm needed maspsx's section handling and does not assemble with GNU as. */
__asm__(".text\n\t.globl FUN_80068ae4\n\t.ent FUN_80068ae4\nFUN_80068ae4:\n\t.set noreorder\n\taddiu $10,$0,0xa0\n\tjr $10\n\taddiu $9,$0,0x72\n\t.set reorder\n\t.end FUN_80068ae4\n");
