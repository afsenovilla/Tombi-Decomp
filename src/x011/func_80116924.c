// FUNC 80116924 564 X011
// MATCHING 80116924 564
/* Debt: register asm("$3") on q, as in the X000 sibling func_80120494 (global-alloc puts it in $6 otherwise). */
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
extern int *DAT_1f800334pA[];
#define DAT_1f800334p DAT_1f800334pA[0]
extern int DAT_1f800334A[];
#define DAT_1f800334 DAT_1f800334A[0]
extern int *DAT_1f800318pA[];
#define DAT_1f800318p DAT_1f800318pA[0]
extern int DAT_1f800318A[];
#define DAT_1f800318 DAT_1f800318A[0]
extern int *DAT_1f800314pA[];
#define DAT_1f800314p DAT_1f800314pA[0]
extern int DAT_1f800314A[];
#define DAT_1f800314 DAT_1f800314A[0]
extern int *DAT_1f800310pA[];
#define DAT_1f800310p DAT_1f800310pA[0]
extern int DAT_1f800310A[];
#define DAT_1f800310 DAT_1f800310A[0]
extern char D_800E3E28[];
extern void FUN_80025aa8(int, int);
extern void func_80027A30(int, int, int);

#define SETUP(o, n, w, v, P, Q) \
    o->wac = w; \
    o->substep++; \
    o->da0 = DAT_1f800334 + DAT_1f800334p[n]; \
    o->velH = v; \
    o->velV = 0; \
    o->b0a = 0x12; \
    p = P; \
    a = o->da0; \
    q = Q; \
    goto tail;

int func_80116924(S *o)
{
    int *p;
    register int q asm("$3");
    int a;
    switch (o->substep) {
    case 0:
        SETUP(o, 1, 0, 0x80, DAT_1f800310p, DAT_1f800310);
    case 2:
        SETUP(o, 2, 1, 0x80, DAT_1f800314p, DAT_1f800314);
    case 4:
        o->wac = 2;
        o->substep++;
        o->da0 = DAT_1f800334 + DAT_1f800334p[3];
        o->velH = 0x80;
        o->velV = 0;
        o->b0a = 0x12;
        p = DAT_1f800318p;
        a = o->da0;
        q = DAT_1f800318;
    tail:
        q += p[1];
        o->db8 = (int)D_800E3E28;
        o->da8 = (int)D_800E3E28;
        o->dbc = q;
        FUN_80025aa8(a, o->db8);
        return 0;
    case 1:
    case 3:
    case 5:
        o->velV -= o->velH;
        if (o->velV < -0x1000) {
            o->velV = -0x1000;
            o->substep++;
        }
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->db8);
        func_80027A30(o->db8, o->dbc, o->wcc);
        return 0;
    case 6:
        o->b0a = 0x11;
        o->substep = 0;
        o->wac = 3;
        o->da0 = DAT_1f800334 + DAT_1f800334p[4];
        return 1;
    }
    return 0;
}
