// JPDEMO slpm80165 80034E10 76
// Demo-only helper: on state 0, sets a flag and clears two words of the D_800893D0 object.
typedef struct { char p[0xc]; unsigned char b0c; char q[0x30 - 0xd]; short w30, w32; } P;
extern P *D_800893D0;
void func_80034E10(unsigned char *o)
{
    if (o[6] == 0) {
        D_800893D0->b0c = 1;
        D_800893D0->w30 = 0;
        D_800893D0->w32 = 0;
        o[6]++;
    }
}
