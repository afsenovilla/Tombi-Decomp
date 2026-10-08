// FUNC 80102110 260 X010
// MATCHING 80102110 260
typedef struct { char p0[4]; unsigned char b4, step, state; char p1[0x2e - 7]; unsigned short af; char p2[0x69 - 0x30]; unsigned char b69; char p3[0x8c - 0x6a]; int d8c; char p4[0x9e - 0x90]; unsigned char b9e; char p5[7]; unsigned char ba6; char p6; unsigned char ba8; } O;
extern void FUN_800fa01c(O *), FUN_80109920(O *), FUN_8010b444(O *), FUN_8010b050(O *), FUN_800f8130(O *), FUN_8003f7cc(O *);

void FUN_80102110(O *o)
{
    switch (o->step) {
    case 0:
        FUN_800fa01c(o);
        break;
    case 3:
        o->af = o->af & 1;
        FUN_80109920(o);
        break;
    case 4:
        o->af = o->af & 1;
        FUN_8010b444(o);
        break;
    case 5:
        o->af = o->af & 1;
        FUN_8010b050(o);
        o->d8c = 0;
        break;
    case 6:
        FUN_800f8130(o);
        break;
    case 7:
        o->d8c = 0;
        FUN_8010b050(o);
        if (o->b69 != 0) {
            o->b4 = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
    o->ba8 = 0;
    o->ba6 = 0;
    if (o->b9e == 0)
        FUN_8003f7cc(o);
}
