// FUNC 80120494 672 X000
typedef struct {
    char p0[5];
    unsigned char step;
    unsigned char state;
    unsigned char substep;
    char p8[2];
    unsigned char b0a;
    char pb[0x80 - 0xb];
    short velH;
    short velV;
    char p84[0xa0 - 0x84];
    int da0;
    char pa4[4];
    int da8;
    short wac;
    char pae[0xb8 - 0xae];
    int db8;
    int dbc;
    char pc0[0xcc - 0xc0];
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

int func_80120494(S *o)
{
    int *p;
    int q;

    switch (o->substep) {
    case 0:
        o->wac = 3;
        o->substep++;
        o->da0 = DAT_1f800334 + DAT_1f800334p[4];
        o->velH = 0x60;
        o->velV = 0;
        o->b0a = 0x12;
        p = DAT_1f800318p;
        q = DAT_1f800318;
        goto tail;
    case 2:
        o->wac = 2;
        o->substep++;
        o->da0 = DAT_1f800334 + DAT_1f800334p[3];
        o->velH = 0x100;
        o->velV = 0;
        o->b0a = 0x12;
        p = DAT_1f800314p;
        q = DAT_1f800314;
        goto tail;
    case 4:
        o->wac = 1;
        o->substep++;
        o->da0 = DAT_1f800334 + DAT_1f800334p[2];
        o->velH = 0x200;
        o->velV = 0;
        o->b0a = 0x12;
        p = DAT_1f800310p;
        q = DAT_1f800310;
        goto tail;
    case 5:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velH = -0x280;
            o->substep++;
        }
        break;
    case 6:
        o->velV += o->velH;
        if (o->velV < 0x800) {
            o->velH = 0x280;
            o->substep++;
        }
        break;
    case 1:
    case 3:
    case 7:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velV = 0x1000;
            o->substep++;
        }
        break;
    case 8:
        o->step = 0;
        o->substep = 0;
        o->b0a = 0x11;
        o->wac = 0;
        o->da0 = DAT_1f800334 + DAT_1f800334p[1];
        return 1;
    default:
        return 0;
    }
    if (0) {
    tail:
        o->db8 = (int)D_800E3E28;
        o->da8 = (int)D_800E3E28;
        o->dbc = q + p[1];
        FUN_80025aa8(o->da0, o->db8);
        return 0;
    }
    o->wcc = o->velV;
    FUN_80025aa8(o->da0, o->db8);
    func_80027A30(o->db8, o->dbc, o->wcc);
    return 0;
}
