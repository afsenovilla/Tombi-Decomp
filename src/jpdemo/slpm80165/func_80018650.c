// JPDEMO slpm80165 80018650 112
// Demo-only helper: walks an 8-byte entry list into func_8001836C while it returns > 0.
typedef struct { unsigned char a, b, c, d; int p; } E;
extern int func_8001836C(char *, int);
void func_80018650(char *o, E *e)
{
    do {
        unsigned char b = e->b, c = e->c;
        *(short *)(o + 0x24) = e->a;
        *(short *)(o + 0x26) = b;
        *(short *)(o + 0x28) = c;
    } while (func_8001836C(o, (e++)->p) > 0);
}
