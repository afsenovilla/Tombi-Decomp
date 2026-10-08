// FUNC 8011935c 268 X000
// MATCHING 8011935c 268
typedef struct P { char p[0x24]; void *anim; char q[0x3c - 0x28]; int w3c; } P;
extern short DAT_8013867c[];
extern void *PTR_DAT_8013b1f4[];
extern int DAT_1f8002d4;
extern short FUN_8005e420(int, int);
extern int FUN_800202b4(char *);
extern void FUN_8001fec0(char *);
extern void FUN_8001fe6c(char *);
extern void FUN_800187e4(char *);

void FUN_8011935c(char *o)
{
    unsigned char s = o[4];

    switch (s) {
    case 0:
        o[4] = s + 1;
        *(short *)(o + 0x1e) = 9;
        *(short *)(o + 8) = FUN_8005e420(0xc0, DAT_8013867c[(unsigned char)o[0xc]]);
        o[0xd] = 1;
        *(signed char *)&o[0xf] = -0x14;
        o[0xa] = 0;
        *(short *)(o + 0x2e) = 0;
        ((P *)o)->w3c = DAT_1f8002d4;
        ((P *)o)->anim = PTR_DAT_8013b1f4[(unsigned char)o[3]];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) != 0)
            FUN_8001fec0(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
