// FUNC 8006bac4 52 MAIN0
/* score 5: only the last store differs: game has lui $at; jr $ra; sw (store in the jr delay slot).
   cc1 2.6.3/2.7.2/2.8.1 (G0/G8, -mgpopt, defined vs extern vars, literal addresses) never fill the
   return delay slot with a 2-insn symbol store; ASPSX 2.86 does not either. Likely library code built
   with another toolchain/assembler (reorder mode).
   gcc-2.8.1 (// CC) with -G0/-G8, -mgpopt, defined vars or literal addresses: still not filled (literal gives 3). */
extern void *DAT_800981c4, *DAT_800981c8, *DAT_800981cc;
extern char LAB_8006baf8[], LAB_8006bf4c[], LAB_8006bc08[];

void FUN_8006bac4(void)
{
    DAT_800981c4 = LAB_8006baf8;
    DAT_800981c8 = LAB_8006bf4c;
    DAT_800981cc = LAB_8006bc08;
}
