// FUNC 8011d010 500 X000
typedef struct {
    char p0[3]; unsigned char sub, st, p1[7]; unsigned char b0c; char p2[0x20 - 0xd]; short w20; char p3[0x2c - 0x22]; unsigned short w2c, w2e;
    char p4[0x69 - 0x30]; unsigned char b69; char p5[0x6c - 0x6a]; unsigned short w6c, w6e, w70, w72; char p6[0x7c - 0x74]; unsigned short w7c, w7e;
    char p7[0x84 - 0x80]; int d84, d88, d8c; char p8[0xa5 - 0x90]; unsigned char ba5, ba6;
} O;
typedef void (*FN)(O *);
extern char *T7490[];
extern FN T7484[], T7478[], T7468[];
extern int FUN_800202b4(O *);
extern void FUN_80018838(O *);

void FUN_8011d010(O *o)
{
    unsigned short *p;
    unsigned char s;
    unsigned short w;

    switch (o->st) {
    case 0:
        p = (unsigned short *)T7490[o->sub] + o->b0c * 8;
        o->w6c = *p++;
        o->w6e = *p++;
        o->w70 = *p++;
        o->w7c = *p;
        o->w72 = *p++;
        o->ba5 = *p++;
        o->ba6 = *p++;
        o->w2c = *p;
        w = p[1];
        s = o->st;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->w2e = 0x80;
        o->w20 = 0;
        o->b69 = 0;
        o->st = s + 1;
        o->w7e = w;
        break;
    case 1:
        if (FUN_800202b4(o)) {
            switch (o->sub) {
            case 0:
                T7484[o->b0c](o);
                break;
            case 1:
                T7478[o->b0c](o);
                break;
            case 2:
                T7468[o->b0c](o);
                break;
            }
        }
        break;
    case 2:
        o->st = o->st + 1;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
