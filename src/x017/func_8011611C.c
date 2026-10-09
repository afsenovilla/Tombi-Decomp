// FUNC 8011611c 1136 X017
// MATCHING 8011611c 1136
/* The csv entry 80115F24 starts with 126 data words (jump tables of other functions); the code starts at
   8011611C and covers the csv pieces 80116258 and 801162B8 too. */
typedef struct { unsigned short frac; short whole; } FP;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x38 - 0x34];
    FP *p38;
    char p3c[0x48 - 0x3c];
    short w48;
} O;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x12 - 8];
    short x;
    char p14[0x1a - 0x14];
    short z;
    char p1c[0xa0 - 0x1c];
    unsigned char ba0;
    char pa1[0xa8 - 0xa1];
    unsigned char ba8;
} PL;
typedef struct { char p[0x4c]; short w4c, w4e; } Q;
typedef void (*Fn)(O *);

extern PL D_800A6038;
extern Fn D_80079AD0[];
extern unsigned char D_8009C93A, D_8009C93C, D_8009C975;
extern unsigned char D_8009D001[];
extern unsigned short D_8009C962;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000F2[];
extern int D_1F8000F0;
extern Q *D_1F8001D4;
extern void func_80027810(O *);
extern int FUN_800270a0(O *, int, int);
extern void FUN_8002715c(int);
extern void FUN_8002a4d0(O *);

void func_8011611C(O *o)
{
    PL *p = &D_800A6038;

    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        o->p38->whole = 0;
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (D_8009C962) {
        case 1:
            if (D_1F800172 >= 0xb5) {
                if (D_8009D001[0] == 0) {
                    D_8009D001[0] = 1;
                    p->b04 = 5;
                    p->step = 0x41;
                    p->visible = 0;
                    p->state = 0;
                    o->step = 3;
                    break;
                }
                FUN_800270a0(o, 1, 1);
            } else if (D_1F800172 < -0x59) {
                FUN_800270a0(o, 1, 0);
            }
            break;
        case 2:
            if ((unsigned short)(p->x - 0x36) < 0x40) {
                p->ba0 = 1;
                p->ba8 = 8;
                if (p->z > 0) {
                    FUN_8002715c(0x56);
                    if (p->z >= 0x5b) {
                        FUN_800270a0(o, 1, 1);
                        break;
                    }
                }
            }
            if ((unsigned short)(p->x - 0x210) < 0x40) {
                p->ba0 = 1;
                p->ba8 = 8;
                if (p->z > 0) {
                    FUN_8002715c(0x230);
                    if (p->z >= 0x5b) {
                        FUN_800270a0(o, 1, 3);
                        break;
                    }
                }
            }
            if ((unsigned short)(p->x - 0x110) < 0x60) {
                if (p->z > 0) {
                    FUN_8002715c(0x140);
                    if (p->z >= 0x5b) FUN_800270a0(o, 1, 2);
                } else if (p->z < 0) {
                    FUN_8002715c(0x140);
                    if (p->z < -0x5a) FUN_800270a0(o, 1, 0);
                }
            }
            break;
        case 3:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x3b) FUN_800270a0(o, 1, 0);
            break;
        case 4:
            if (D_1F80016A >= 0x12d && D_1F80016E >= -0x3b) FUN_800270a0(o, 1, 0);
            break;
        case 5:
            if (D_1F80016A >= 0x12d && D_1F80016E >= -0x40) FUN_800270a0(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    case 3:
        D_1F8000F2[0]--;
        if (D_1F8000F2[0] < o->w30) {
            D_1F8000F0 = o->w30 << 16;
            o->w48 = 0x1e;
            o->step++;
        }
        break;
    case 4:
        if (--o->w48 == -1) {
            o->w48 = 0x3c;
            o->step++;
        }
        break;
    case 5:
        if (--o->w48 == -1) {
            D_8009CD94 = 0x11;
            D_8009CD96 = 2;
            D_8009CDA0 = 0;
            D_8009C93C = 0;
            D_8009C975 = 3;
            o->step++;
        }
    case 6:
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        D_1F8000F2[0]++;
        if (o->w32 < D_1F8000F2[0]) D_1F8000F0 = o->w32 << 16;
        break;
    }
}
