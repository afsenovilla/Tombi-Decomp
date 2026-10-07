// FUNC 80125c1c 104 X000
// MATCHING 80125c1c 104
typedef struct A { char pad[2]; unsigned char t; } A;
typedef struct B { char pad[4]; unsigned char s; unsigned char u; } B;
extern int func_8004461c(void);

void FUN_80125c1c(A *a, B *b)
{
    if (b->s == 0 && func_8004461c() << 16 >= 0 && a->t != 8) {
        b->s = 1;
        b->u = 0;
    }
}
