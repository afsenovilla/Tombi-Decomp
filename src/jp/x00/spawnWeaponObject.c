// JP x00 800EEB04 132
// Japanese version of PAL x000 800EE948 (spawnWeaponObject.c): 227 -> 211.
extern unsigned char DAT_8009d2b2[];
extern short DAT_8007a038[];
extern unsigned char *allocObjectLayer1(void);
void spawnWeaponObject(signed char *o, unsigned char p)
{
    unsigned char *q;
    int pad[1];
    if (o[0xd3] < DAT_8007a038[DAT_8009d2b2[0]]) {
        o[0xd3]++;
        q = allocObjectLayer1();
        if (q != 0) {
            q[0] = 1;
            q[2] = DAT_8009d2b2[0];
            q[5] = p;
            q[6] = 0;
        }
    }
}
