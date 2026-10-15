// JPDEMO x00 800EB5D4 40
// Demo-only helper: in section 2, sets +0x88 to 0xa00 and state 2.
extern unsigned short D_8009A812;
void func_800EB5D4(char *o)
{
    if (D_8009A812 == 2) {
        *(int *)(o + 0x88) = 0xa00;
        o[6] = 2;
    }
}
