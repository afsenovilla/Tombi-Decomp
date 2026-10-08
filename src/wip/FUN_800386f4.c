// FUNC 800386f4 236 MAIN0
typedef struct { char c[4]; } B4;
typedef struct { char p[0x89]; unsigned char r; unsigned short i; char q[0x1090 - 0x8c]; int t[1]; } O;
extern O *DAT_8009f0f0;
extern unsigned char *DAT_8009d60c;

void FUN_800386f4(unsigned char m)
{
    O *o = DAT_8009f0f0;
    int a, b;
    unsigned char *p;
    B4 s;
    a = o->t[DAT_8009d60c[o->i + 1]];
    p = DAT_8009d60c + o->i + 2;
    if (m == 0) {
        b = DAT_8009f0f0->t[*p];
    } else {
        s = *(B4 *)p;
        b = *(int *)&s;
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
