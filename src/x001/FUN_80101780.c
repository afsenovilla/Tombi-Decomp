// FUNC 80101780 236 X001
// MATCHING 80101780 236
typedef struct O {
    char pad0[4];
    unsigned char b04;
    unsigned char step;
    char pad1[0x9e - 6];
    unsigned char b9e;
    char pad2[0xa6 - 0x9f];
    unsigned char ba6;
    char pad3;
    unsigned char wa8;
} O;
extern void FUN_800fd358(), FUN_800fdb7c(), FUN_800fe13c(), FUN_800fe75c(), FUN_800fe98c(), FUN_8003f7cc();

void FUN_80101780(O *o)
{
    register char r;
    switch (o->step) {
    case 0: FUN_800fd358(o); break;
    case 1: FUN_800fdb7c(o); break;
    case 2: FUN_800fe13c(o); break;
    case 3: FUN_800fe75c(o); break;
    case 4: FUN_800fe98c(o); break;
    }
    r = 0;
    o->wa8 = 0;
    o->ba6 = 0;
    if (o->b9e == 0 && o->b04 == 5) {
        r = o->step == 0x40;
        if (o->step == 0x65)
            r++;
    }
    if (!r)
        FUN_8003f7cc(o);
}
