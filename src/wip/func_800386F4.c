// FUNC 800386f4 236 MAIN0
// wip r8: score 4. Only diff: game puts addiu sp,-8 right after the first lbu (load delay); ours schedules it after the second lhu. Second extern name D_8009F0F0b gives the reload before the branch.
typedef struct { char p[0x89]; unsigned char r; unsigned short i; char q[0x1090 - 0x8c]; int t[1]; } O;
extern O *D_8009F0F0;
extern O *D_8009F0F0b;
extern unsigned char *D_8009D60C;


void func_800386F4(unsigned char m)
{
    O *o = D_8009F0F0;
    int a, b;
    unsigned char *p;
    O *o2;
    a = *(volatile int *)&o->t[D_8009D60C[o->i + 1]];
    p = D_8009D60C + *(volatile unsigned short *)&o->i;
    p += 2;
    o2 = D_8009F0F0b;
    if (m == 0) {
        b = o2->t[*p];
    } else {
        { int s; int k; for (k = 0; k < 4; k++) ((char *)&s)[k] = *p++; b = s; }
    }
    if (a == b)
        o->r = 0;
    else if (a < b)
        o->r = 1;
    else
        o->r = 2;
    if (m == 0)
        o->i += 3;
    else
        o->i += 6;
}
