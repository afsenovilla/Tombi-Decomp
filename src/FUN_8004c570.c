// FUNC 8004c570 344 MAIN0
// MATCHING 8004c570 344
typedef struct { signed char active; char x; unsigned char type; char pad[12]; unsigned char b0f; } E;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern E **DAT_8007b908[];
extern short DAT_80078790[];
extern void FUN_8004cb00(E *, int);
extern int FUN_80020a78(int);

void FUN_8004c570(void)
{
    E ***tab = (E ***)DAT_8007b908[DAT_8009c960];
    E *p;
    unsigned char *q;
    short n;
    int r;
    char pad;
    unsigned b;
    for (p = *tab[DAT_8009c962 & 7]; p->type < 0xff; p++)
        FUN_8004cb00(p, -1);
    n = DAT_80078790[DAT_8009c960];
    if (n > 15) n = 15;
    p = *tab[n];
    q = &p->type;
    while (*q < 0xff) {
        b = q[0xd] >> 2;
        r = FUN_80020a78(b);
        if (r == 0) {
            if (p->active == DAT_8009c962 || p->active == ~DAT_8009c962)
                FUN_8004cb00(p, b);
        }
        q += 16;
        p++;
    }
}
