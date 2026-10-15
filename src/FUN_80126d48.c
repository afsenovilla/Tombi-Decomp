// FUNC 80126d48 52 X000
// MATCHING 80126d48 52
extern short pushObjectOutOfObjectBoxB();

void FUN_80126d48(void)
{
    if (pushObjectOutOfObjectBoxB() != 0)
        *(short *)0x1f80019e = 0;
}
