/* score 25 (ncheck now; old comment said 12 under older scoring, that version scores 35): only case 0's order differs.
   Game: w74, w78 stores early, then loads D_1F80031C twice (first copy used for ->d4); ours delays the w78 store into a load
   delay slot and uses the second load for ->d4. Tried two extern names, single `H *volatile` extern (this), d4-first operand
   order, k temp, hill-climb of all case-0 statements. */
typedef struct L { char p[0x30]; int d30; char q[0x94 - 0x34]; struct L *next; } L;
typedef struct { char p[4]; int d4; } H;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[2];
    unsigned char b0a;
    char p0b[0x20 - 0xb];
    short timer;
    char p22[0x74 - 0x22];
    short w74, w76, w78;
    char p7a[0x94 - 0x7a];
    L *next;
    char p98[0xa0 - 0x98];
    int da0;
    char pa4[4];
    int da8;
    char pac[0xc4 - 0xac];
    int dc4, dc8;
    short wcc;
} O;
extern H *volatile D_1F80031C;
extern char D_800E3E28[];
extern void FUN_80025aa8(int, int);
extern void func_80027A30(int, int, int);

int func_8011AFB4(O *o)
{
    L *p;
    H *h;
    int k;

    switch (o->substep) {
    case 0:
        o->w76 = 0;
        o->w74 = 0x300;
        o->w78 = -0x48;
        o->b0a = 0x16;
        o->substep++;
        o->dc8 = (int)D_1F80031C + D_1F80031C->d4;
        o->dc4 = (int)D_800E3E28;
        o->da8 = (int)D_800E3E28;
        FUN_80025aa8(o->da0, o->dc4);
        return 0;
    case 1:
        o->w76 += o->w74;
        o->w74 += o->w78;
        p = o->next;
        p->d30 += -0xa000;
        p = p->next;
        p->d30 += -0xa000;
        p = p->next;
        p->d30 += 0xa000;
        p = p->next;
        p->d30 += 0xa000;
        if (o->w76 > 0x1000) {
            o->w76 = 0x1000;
            o->timer = 0xc;
            o->substep++;
        }
        goto common;
    case 2:
        if (--o->timer == -1) {
            o->w74 = 0x300;
            o->w78 = -0x48;
            o->substep++;
        }
        return 0;
    case 3:
        o->w76 -= o->w74;
        o->w74 += o->w78;
        p = o->next;
        p->d30 += 0xa000;
        p = p->next;
        p->d30 += 0xa000;
        p = p->next;
        p->d30 -= 0xa000;
        p = p->next;
        p->d30 -= 0xa000;
        if (o->w76 < 0) {
            o->w76 = 0;
            o->substep++;
        }
    common:
        o->wcc = o->w76;
        FUN_80025aa8(o->da0, o->dc4);
        func_80027A30(o->dc4, o->dc8, o->wcc);
        return 0;
    case 4:
        o->b0a = 0x15;
        o->substep = 0;
        return 1;
    }
    return 0;
}
