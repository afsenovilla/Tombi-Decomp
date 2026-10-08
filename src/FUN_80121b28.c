// FUNC 80121b28 808 X000
// MATCHING 80121b28 808
typedef struct P {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned char b4;
    char pad5[7];
    unsigned char bc;
    char padd[3];
    int d10;
    int d14;
    int d18;
} P;
typedef struct O {
    char pad0[6];
    unsigned char state;
    char pad7[0x22 - 7];
    short w22;
    char pad24[0x69 - 0x24];
    unsigned char b69;
    char pad6a[0xb4 - 0x6a];
    P *p[6];
} O;
extern unsigned char DAT_800a60da;
extern P *FUN_80018448();

void FUN_80121b28(O *o)
{
    P *p;
    unsigned char one;
    switch (o->state) {
    case 0:
        if (o->b69 == 1 && DAT_800a60da == 1) {
            one = 1;
            p = FUN_80018448();
            if (p != 0) {
                p->b2 = 0x44;
                p->d10 = 0xa3c0000;
                p->d14 = 0xfea20000;
                p->b0 = one;
                p->bc = one;
                p->d18 = 0x9e0000;
            }
            o->p[0] = p;
            p = FUN_80018448();
            if (p != 0) {
                p->b2 = 0x44;
                p->d10 = 0xa3c0000;
                p->d14 = 0xfea20000;
                p->b0 = one;
                p->bc = 0;
                p->d18 = 0xae0000;
            }
            o->p[1] = p;
            p = FUN_80018448();
            if (p != 0) {
                p->b2 = 0x44;
                p->d10 = 0xa3c0000;
                p->d14 = 0xfea20000;
                p->b0 = one;
                p->bc = 0;
                p->d18 = 0xbe0000;
            }
            o->p[2] = p;
            p = FUN_80018448();
            if (p != 0) {
                p->b2 = 0x44;
                p->d10 = 0xa3c0000;
                p->d14 = 0xfea20000;
                p->b0 = one;
                p->bc = 0;
                p->d18 = 0xce0000;
            }
            o->p[3] = p;
            p = FUN_80018448();
            if (p != 0) {
                p->b2 = 0x44;
                p->d10 = 0xa3c0000;
                p->d14 = 0xfea20000;
                p->b0 = one;
                p->bc = 0;
                p->d18 = 0xde0000;
            }
            o->p[4] = p;
            p = FUN_80018448();
            if (p != 0) {
                p->b2 = 0x44;
                p->d10 = 0xa3c0000;
                p->d14 = 0xfea20000;
                p->b0 = one;
                p->bc = 0;
                p->d18 = 0xee0000;
            }
            o->p[5] = p;
            o->state++;
        }
        break;
    case 1:
        if (o->b69 == 1) {
            if (DAT_800a60da == 1) break;
            o->p[0]->b4 = 2;
            o->p[1]->b4 = 2;
            o->p[2]->b4 = 2;
            o->p[3]->b4 = 2;
            o->p[4]->b4 = 2;
            o->p[5]->b4 = 2;
            o->w22 = 0x1e;
            o->state++;
        }
        o->p[0]->b4 = 2;
        o->p[1]->b4 = 2;
        o->p[2]->b4 = 2;
        o->p[3]->b4 = 2;
        o->p[4]->b4 = 2;
        o->p[5]->b4 = 2;
        o->w22 = 0x1e;
        o->state++;
        break;
    case 2:
        if (--o->w22 == -1) o->state++;
    case 3:
        if (o->b69 != 1 || DAT_800a60da != 1) o->state = 0;
        break;
    }
}
