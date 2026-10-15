// FUNC 8006bac4 52 MAIN0
// MATCHING 8006bac4 52
/* Library leaf: three function-pointer globals set to local labels. The game has the second half of the expanded
   `sw $v0,sym` macro (lui at; jr ra; sw) in the jr delay slot, which gcc 2.6.3/2.7.2/2.8.1 + ASPSX never produce
   from C (score 5 with any flags, see _padChkVsync). Written as a top-level asm body (debt) to be byte-exact.
   C equivalent: DAT_800981c4 = LAB_8006baf8; DAT_800981c8 = LAB_8006bf4c; DAT_800981cc = LAB_8006bc08; */
extern void *DAT_800981c4, *DAT_800981c8, *DAT_800981cc;
extern char LAB_8006baf8[], LAB_8006bf4c[], LAB_8006bc08[];
__asm__(".text\n.align 2\n.globl _padInitDirSeq\n.ent _padInitDirSeq\n_padInitDirSeq:\n"
        ".set noreorder\n.set noat\n"
        "lui $2,%hi(LAB_8006baf8)\naddiu $2,$2,%lo(LAB_8006baf8)\n"
        "lui $1,%hi(DAT_800981c4)\nsw $2,%lo(DAT_800981c4)($1)\n"
        "lui $2,%hi(LAB_8006bf4c)\naddiu $2,$2,%lo(LAB_8006bf4c)\n"
        "lui $1,%hi(DAT_800981c8)\nsw $2,%lo(DAT_800981c8)($1)\n"
        "lui $2,%hi(LAB_8006bc08)\naddiu $2,$2,%lo(LAB_8006bc08)\n"
        "lui $1,%hi(DAT_800981cc)\njr $31\nsw $2,%lo(DAT_800981cc)($1)\n"
        ".set at\n.set reorder\n.end _padInitDirSeq\n");
