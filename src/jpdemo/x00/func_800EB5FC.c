// JPDEMO x00 800EB5FC 40
// Demo-only helper: in section 2, sets +0x88 to 0x600 and state 2.
extern unsigned short D_8009A812;
void func_800EB5FC(char *o)
{
    if (D_8009A812 == 2) {
        *(int *)(o + 0x88) = 0x600;
        o[6] = 2;
    }
}
