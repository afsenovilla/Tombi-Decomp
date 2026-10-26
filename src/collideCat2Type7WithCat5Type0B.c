// FUNC 800490ac 324 MAIN0
// MATCHING 800490ac 324
// VERSION SLPM_80165 slpm80165 8004DA84 324
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 800490AC (FUN_800490ac.c): tests +3 before the type.
extern int GAME;
extern short DAT_1f800246;
extern unsigned char **DAT_1f80021c;
extern unsigned short DAT_1f800248;
extern unsigned char **DAT_1f800228;
extern short DAT_1f80019e;
extern void collideCat2Type7Cat5Type0BPair(unsigned char *a, unsigned char *b);

void FUN_800490ac(void)
{
    short n;
    unsigned char **pp, **qq;
    unsigned char *o, *e;
    if (GAME == 0x10000) {
        n = DAT_1f800246;
        pp = DAT_1f80021c;
        while (n != 0) {
            o = *pp++;
            n--;
            if ((o[0] & 3) && o[3] != 0 && o[2] == 7 && o[0xc] == 1 && o[0x69] == 0) {
                qq = DAT_1f800228;
                for (DAT_1f80019e = DAT_1f800248; DAT_1f80019e != 0; ) {
                    e = *qq++;
                    DAT_1f80019e--;
                    if ((e[0] & 3) && e[2] == 0xb) {
                        collideCat2Type7Cat5Type0BPair(o, e);
                        break;
                    }
                }
            }
        }
    }
}
#else
extern int GAME;
extern short DAT_1f800246;
extern unsigned char **DAT_1f80021c;
extern unsigned short DAT_1f800248;
extern unsigned char **DAT_1f800228;
extern short DAT_1f80019e;
extern void collideCat2Type7Cat5Type0BPair(unsigned char *a, unsigned char *b);

void collideCat2Type7WithCat5Type0B(void)
{
    short n;
    unsigned char **pp, **qq;
    unsigned char *o, *e;
    if (GAME == 0x10000) {
        n = DAT_1f800246;
        pp = DAT_1f80021c;
        while (n != 0) {
            o = *pp++;
            n--;
            if ((o[0] & 3) && o[2] == 7 && o[3] != 0 && o[0xc] == 1 && o[0x69] == 0) {
                qq = DAT_1f800228;
                for (DAT_1f80019e = DAT_1f800248; DAT_1f80019e != 0; ) {
                    e = *qq++;
                    DAT_1f80019e--;
                    if ((e[0] & 3) && e[2] == 0xb) {
                        collideCat2Type7Cat5Type0BPair(o, e);
                        break;
                    }
                }
            }
        }
    }
}
#endif
