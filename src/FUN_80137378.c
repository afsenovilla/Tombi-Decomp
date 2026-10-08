// FUNC 80137378 224 X000
// MATCHING 80137378 224
typedef struct N { char p0; char f1; char f2; char f3; char p1[0xe]; short x; char p2[2]; short y; char p3[2]; short z; char p4[0x2e - 0x1c]; short w; } N;
typedef struct O { char p0[4]; unsigned char state; char p1[0x1c-5]; N *n; } O;
extern unsigned char DAT_8009ce4a, DAT_8009cebd, DAT_8009d00e;
extern N *FUN_800183b8(void);

void FUN_80137378(O *o)
{
    N **pp = &o->n;
    N *n;
    unsigned char st;
    if (DAT_8009ce4a == 0xff)
        o->state = 3;
    if (DAT_8009cebd >= 6) st = 3; else {
        n = FUN_800183b8();
        if (n) {
            n->p0 = 1;
            n->f2 = 0x15;
            if (DAT_8009d00e == 0) {
                n->w = 1;
                n->x = 0x140;
                n->y = -0x32;
                n->z = 0;
                n->f3 = 99;
            } else {
                n->x = 0x138;
                n->w = 0;
                n->y = -0x11c;
                n->z = 0;
                n->f3 = 0;
            }
            *pp = n;
        }
        st = 1;
    }
    o->state = st;
}
