// FUNC 80119468 284 X000
// MATCHING 80119468 284
typedef struct P { char p[0x24]; void *anim; char q[0x3c - 0x28]; int w3c; } P;
extern int DAT_1f8002d4;
extern char DAT_8013b810[];
extern int FUN_800202b4(char *);
extern void FUN_80020490(char *);
extern int FUN_8001fec0(char *);
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(char *);
extern void FUN_800187e4(char *);

void FUN_80119468(char *o)
{
    switch ((unsigned char)o[4]) {
    case 0:
        o[4]++;
        *(short *)(o + 0x1e) = 10;
        *(short *)(o + 8) = FUN_8005e420(0xe0, 0x1e4);
        o[0xd] = 1;
        o[0xa] = 0;
        o[3] = 0;
        *(signed char *)&o[0xf] = -13;
        *(short *)(o + 0x2e) = 0;
        ((P *)o)->anim = DAT_8013b810;
        ((P *)o)->w3c = DAT_1f8002d4;
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) == 0)
            FUN_80020490(o);
        if (FUN_8001fec0(o) != 0)
            o[4]++;
        break;
    case 2:
        o[4]++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
