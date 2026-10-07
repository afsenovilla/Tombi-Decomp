// FUNC 80017414 104 MAIN0
// MATCHING 80017414 104
typedef struct E { unsigned short s; unsigned short n; char pad[0x6c]; } E;
void FUN_80017414(void)
{
E *p = (E *)0x801fd800;
do {
    if (p->s == 1) {
        p->n--;
        if ((short)p->n == 0) p->s = 2;
    }
    p++;
} while (p <= (E *)0x801fd94f);
}
