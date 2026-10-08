// FUNC 8004baa8 220 MAIN0
// MATCHING 8004baa8 220
extern int DAT_1f8003c0;
extern short DAT_1f80019e;

extern unsigned short DAT_1f800254;
extern unsigned char **DAT_1f800268;
extern void FUN_8004b7c4(void *, unsigned char *);

int FUN_8004baa8(void *o)
{
    unsigned char **q = DAT_1f800268;
    unsigned char *p;
    DAT_1f8003c0 = 0;
    DAT_1f80019e = DAT_1f800254;
    while (DAT_1f80019e != 0) {
        p = *q;
        DAT_1f80019e = DAT_1f80019e - 1;
        q++;
        if (p[0] & 1) {
            switch (p[2]) {
            case 0x10: case 0x11: case 0x19: case 0x35: case 0x37: case 0x3d: case 0x3e:
                FUN_8004b7c4(o, p);
            }
        }
    }
    return DAT_1f8003c0;
}
