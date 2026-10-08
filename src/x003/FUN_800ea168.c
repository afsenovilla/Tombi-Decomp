// FUNC 800ea168 184 X003
// MATCHING 800ea168 184
typedef struct { char p0[4]; unsigned char b4, b5; char p1[0x2e - 6]; short s2e; } TO;
extern void g1(TO *o);
extern void g2(TO *o);
extern void g3(TO *o);

void FUN_800ea168(TO *o)
{
    switch (o->b4) {
    case 0:
        g1(o);
        o->s2e = 0;
        o->b4++;
    case 1:
        g2(o);
        if (o->b5 == 0) o->b5++;
        break;
    case 2:
        o->b4++;
        break;
    case 3:
        g3(o);
        break;
    }
}
