// FUNC 8005d4ec 24 MAIN0
// MATCHING 8005d4ec 24
/* delay loop; the count is loaded as a symbol address (D_31FFFF = 0x31FFFF) */
/* Debt pass: kept asm("t6"): a lone leaf temp always gets v0 and li gives lui/ori, so this is most likely a hand-written asm routine. */
extern char D_31FFFF[];

void func_8005D4EC(void)
{
    register int i asm("t6") = (int)D_31FFFF;
loop:
    if (i != 0) { i--; goto loop; }
}
