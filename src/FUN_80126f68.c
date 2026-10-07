// FUNC 80126f68 132 X000
// MATCHING 80126f68 132
typedef struct O {
    unsigned char active;
    char pad0[0x69 - 1];
    unsigned char b69;
    unsigned char b6a;
    char pad1[0x90 - 0x6b];
    struct O *d90;
    struct O *d94;
} O;
extern short FUN_80048ffc();
extern short DAT_1f80019e;

void FUN_80126f68(O *o, unsigned char *p)
{
    O *a, *b;
    if (FUN_80048ffc(o) != 0) {
        a = o->d90;
        b = o->d94;
        o->b6a = 1;
        o->b69 = 2;
        a->b69 = 2;
        b->b69 = 2;
        o->active = 2;
        a->active = 2;
        b->active = 2;
        p[0] = 4;
        p[4] = 2;
        p[5] = 1;
        p[6] = 0;
        DAT_1f80019e = 0;
    }
}
