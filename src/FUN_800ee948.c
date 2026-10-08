// FUNC 800ee948 132 X000
// MATCHING 800ee948 132
extern unsigned char DAT_8009d2b2[];
extern short DAT_8007a038[];
extern unsigned char *FUN_800182ac(void);
void FUN_800ee948(signed char *o, unsigned char p)
{
    unsigned char *q;
    int pad[1];
    if (o[0xe3] < DAT_8007a038[DAT_8009d2b2[0]]) {
        o[0xe3]++;
        q = FUN_800182ac();
        if (q != 0) {
            q[0] = 1;
            q[2] = DAT_8009d2b2[0];
            q[5] = p;
            q[6] = 0;
        }
    }
}
