// FUNC 80135a14 308 X001
// MATCHING 80135a14 308
#include "TOBJ.H"

extern unsigned char D_8009CEC1[], D_8009CECC[], D_8009CECD[];
extern int Rand(void);

void func_80135A14(TObj *o)
{
    short i;
    char pad[16];

    i = 0;
    while (D_8009CEC1[i] && i < 7) i++;
    if (i >= 7) {
        D_8009CEC1[o->b0c + 11] = (Rand() & 1) - 0x64;
    } else {
        D_8009CEC1[o->b0c + 11] = i - 0x6c;
        D_8009CEC1[i] = 1;
    }
    for (i = 0; i < 7; i++) {
        if (D_8009CEC1[i + 12] == o->b0c + 0x93) D_8009CEC1[i + 12] = (Rand() & 1) - 0x64;
    }
}
