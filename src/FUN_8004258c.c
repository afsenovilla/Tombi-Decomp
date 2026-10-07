// FUNC 8004258c 56 MAIN0
typedef struct { char p0[0x98]; short t; } TO;

void FUN_8004258c(TO *o, unsigned char d)
{
    if (o->t) {
        o->t -= d;
        if (o->t <= 0) o->t = 0;
    }
}
