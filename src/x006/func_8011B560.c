// FUNC 8011b560 836 X006
// MATCHING 8011b560 836
/* Sibling of X000 func_80120494. Debt: register asm("$3") on q (same as the sibling); raw-offset wac store in RET8 keeps the scalar loads after it. */
typedef struct {
    char p0[7];
    unsigned char substep;
    char p8[2];
    unsigned char b0a;
    char pb[0x74 - 0xb];
    short velH;
    short velV;
    char p78[0xa0 - 0x78];
    int da0;
    char pa4[4];
    int da8;
    short wac;
    char pae[0xc4 - 0xae];
    int dc4;
    int dc8;
    short wcc;
} S;
extern int *DAT_1f800334p;
extern int DAT_1f800334;
extern int *DAT_1f800318p;
extern int DAT_1f800318;
extern int *DAT_1f800314p;
extern int DAT_1f800314;
extern int *DAT_1f800310p;
extern int DAT_1f800310;
extern char D_800E3E28[];
extern void FUN_80025aa8(int, int);
extern void func_80027A30(int, int, int);

#define RET(w, n) { o->velV = 0x1000; o->wac = w; o->da0 = DAT_1f800334 + DAT_1f800334p[n]; o->b0a = 0x15; o->substep = 0; return 1; }
#define RET8 { o->velV = 0x1000; *(short *)((char *)o + 0xac) = 0; o->da0 = DAT_1f800334 + DAT_1f800334p[1]; o->b0a = 0x15; o->substep = 0; return 1; }
int func_8011B560(S *o)
{
    int *p;
    register int q asm("$3");
    int a;

    switch (o->substep) {
    case 0:
        if (o->wac == 0) {
            o->substep = 1;
        } else if (o->wac == 1) {
            o->substep = 3;
        } else if (o->wac == 2) {
            o->substep = 5;
        } else {
            return 0;
        }
        break;
    case 1:
        a = o->da0;
        o->velH = 0x60;
        o->substep++;
        o->velV = 0;
        o->b0a = 0x16;
        p = DAT_1f800318p;
        q = DAT_1f800318;
        goto tail;
    case 2:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            RET(2, 3);
        }
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->dc4);
        func_80027A30(o->dc4, o->dc8, o->wcc);
        return 0;
    case 3:
        a = o->da0;
        o->velH = 0x100;
        o->substep++;
        o->velV = 0;
        o->b0a = 0x16;
        p = DAT_1f800314p;
        q = DAT_1f800314;
        goto tail;
    case 4:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            RET(1, 2);
        }
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->dc4);
        func_80027A30(o->dc4, o->dc8, o->wcc);
        return 0;
    case 5:
        a = o->da0;
        o->velH = 0x200;
        o->substep++;
        o->velV = 0;
        o->b0a = 0x16;
        p = DAT_1f800310p;
        q = DAT_1f800310;
    tail:
        q += p[1];
        o->dc4 = (int)D_800E3E28;
        o->da8 = (int)D_800E3E28;
        o->dc8 = q;
        FUN_80025aa8(a, o->dc4);
        return 0;
    case 6:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velH = -0x280;
            o->substep++;
        }
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->dc4);
        func_80027A30(o->dc4, o->dc8, o->wcc);
        return 0;
    case 7:
        o->velV += o->velH;
        if (o->velV < 0x800) {
            o->velH = 0x280;
            o->substep++;
        }
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->dc4);
        func_80027A30(o->dc4, o->dc8, o->wcc);
        return 0;
    case 8:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            RET8;
        }
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->dc4);
        func_80027A30(o->dc4, o->dc8, o->wcc);
        return 0;
    }
    return 0;
}
