// FUNC 800697a0 20 MAIN0
// MATCHING 800697a0 20
/* Library leaf: v = D; D = 0; return v. The game has the second half of the expanded `sw $0,sym` macro
   (lui at; jr ra; sw) in the jr delay slot, which gcc 2.7.2/2.8.1 + ASPSX never produce from C (score 3-9 with
   any flags). Written as a top-level asm body (debt) to be byte-exact. */
extern int D_80098214;
__asm__(".text\n.align 2\n.globl _padChkVsync\n.ent _padChkVsync\n_padChkVsync:\n"
        ".set noreorder\n.set noat\n"
        "lui $2,%hi(D_80098214)\nlw $2,%lo(D_80098214)($2)\n"
        "lui $1,%hi(D_80098214)\njr $31\nsw $0,%lo(D_80098214)($1)\n"
        ".set at\n.set reorder\n.end _padChkVsync\n");
