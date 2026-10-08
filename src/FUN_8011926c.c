// FUNC 8011926c 240 X000
// MATCHING 8011926c 240
typedef struct P { char p[0x24]; int w24; char q[0x3c - 0x28]; int w3c; } P;
extern int DAT_1f8002d4;
extern int DAT_8013b1f0;
extern int FUN_800202b4(char *);
extern int FUN_8001fec0(char *);
extern void FUN_8001fe6c(char *);
extern void FUN_8001e4f0(int);
extern void FUN_800187e4(char *);

void FUN_8011926c(char *o)
{
    unsigned char s = o[4];
    switch (s) {
    case 0:
        o[4] = s + 1;
        *(short *)(o + 0x1e) = 8;
        o[0xd] = 0;
        o[0xa] = 0;
        o[3] = 0;
        *(signed char *)&o[0xf] = -8;
        *(short *)(o + 0x2e) = 0;
        ((P *)o)->w3c = DAT_1f8002d4;
        ((P *)o)->w24 = DAT_8013b1f0;
        FUN_8001fe6c(o);
        FUN_8001e4f0(0x30);
        break;
    case 1:
        if (FUN_800202b4(o) == 0 || FUN_8001fec0(o) != 0)
            o[4] = 3;
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
