// FUNC 800697a0 20 MAIN0
// wip: the game fills the jr $ra delay slot with the second half of the expanded "sw $0,sym" macro (lui at; jr ra; sw); gcc+ASPSX leave it unfilled (score 3-5). Library code built with assembler-level reordering? Not reproducible with plain C so far.
extern int D_80098214;

int func_800697A0(void)
{
    int v = D_80098214;
    D_80098214 = 0;
    return v;
}
