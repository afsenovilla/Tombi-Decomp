// FUNC 8006bac4 52 MAIN0
// MATCHING 8006bac4 52
/* Library leaf: three function-pointer globals set to local labels. The game has the second half of the expanded
   `sw $v0,sym` macro (lui at; jr ra; sw) in the jr delay slot, which gcc 2.6.3/2.7.2/2.8.1 + ASPSX never produce
   from C (score 5 with any flags, see _padChkVsync). Written as a top-level asm body (debt) to be byte-exact.
   C equivalent: _padFuncSendAuto = LAB_8006baf8; _padFuncChkEng = LAB_8006bf4c; _padFuncRecvAuto = LAB_8006bc08; */
extern void *_padFuncSendAuto, *_padFuncChkEng, *_padFuncRecvAuto;
extern char LAB_8006baf8[], LAB_8006bf4c[], LAB_8006bc08[];
__asm__(".text\n.align 2\n.globl _padInitDirSeq\n.ent _padInitDirSeq\n_padInitDirSeq:\n"
        ".set noreorder\n.set noat\n"
        "lui $2,%hi(LAB_8006baf8)\naddiu $2,$2,%lo(LAB_8006baf8)\n"
        "lui $1,%hi(_padFuncSendAuto)\nsw $2,%lo(_padFuncSendAuto)($1)\n"
        "lui $2,%hi(LAB_8006bf4c)\naddiu $2,$2,%lo(LAB_8006bf4c)\n"
        "lui $1,%hi(_padFuncChkEng)\nsw $2,%lo(_padFuncChkEng)($1)\n"
        "lui $2,%hi(LAB_8006bc08)\naddiu $2,$2,%lo(LAB_8006bc08)\n"
        "lui $1,%hi(_padFuncRecvAuto)\njr $31\nsw $2,%lo(_padFuncRecvAuto)($1)\n"
        ".set at\n.set reorder\n.end _padInitDirSeq\n");
