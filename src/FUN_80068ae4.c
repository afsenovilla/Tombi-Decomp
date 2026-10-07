// FUNC 80068ae4 12 MAIN0
// MATCHING 80068ae4 12
void FUN_80068ae4(void)
{
    asm(".set noreorder\n\taddiu $10,$0,0xa0\n\tjr $10\n\taddiu $9,$0,0x72\n\t.set reorder\n\t.data");
}
