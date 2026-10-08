// FUNC 80106a18 312 X005
// MATCHING 80106a18 312
typedef struct {
    char p0[5]; unsigned char a5, a6, a7; char p1[0x20 - 8]; short s20; char p2[0x2e - 0x22]; unsigned short af;
    char p3[0x69 - 0x30]; unsigned char b69; char p4[0x8c - 0x6a]; int d8c; char p5[0x9c - 0x90]; unsigned char b9c, b9d;
    char p6[0xac - 0x9e]; unsigned char bac; char p7[3]; short wb0, wb2; char p8[2]; short wb6; char p9[0xc6 - 0xb8];
    unsigned char bc6, bc7; char pa[0xe3 - 0xc8]; unsigned char be3;
} O;
typedef struct { unsigned char z; char p[0x1f]; short s20; } P;
typedef struct { char p[2]; unsigned char k; } K;
extern P *DAT_8009c330;
extern K *DAT_8009d2e8;
extern void FUN_800eeb5c(O *, int);
extern void FUN_800eee90(O *);
extern void FUN_8011c758(O *);
extern void FUN_8011d5f4(O *);
extern void FUN_8011b47c(O *);

void FUN_80106a18(O *o)
{
    if (o->a6 == 0) {
        DAT_8009c330->s20 = 0;
        o->bc7 = 1;
        o->b9d = 0;
        o->bc6 = 0;
        o->be3 = 0;
        o->s20 = 0;
        o->wb2 = 0;
        o->wb6 = 0;
        o->wb0 = 0;
        o->d8c = 0;
        o->af = o->af & 1;
        DAT_8009c330->z = 0;
        FUN_800eeb5c(o, 13);
        o->a7 = 0;
        o->a6 = o->a6 + 1;
    } else {
        switch (DAT_8009d2e8->k) {
        case 0xe:
            FUN_8011c758(o);
            break;
        case 0x27:
            FUN_8011d5f4(o);
            break;
        case 0x3d:
            FUN_8011b47c(o);
            break;
        default:
            FUN_800eee90(o);
            o->b9c = 0;
            o->b69 = 0;
            o->bac = 0;
            o->wb2 = 0;
            o->a5 = 0x3e;
            o->a6 = 0;
            break;
        }
    }
}
