// VERSION SLPM_80165 slpm80165 8004FAAC 332
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8004C570 (FUN_8004c570.c): the current section's list comes from the scratchpad table at 0x1F800358.
typedef struct { signed char active; char x; unsigned char type; char pad[12]; unsigned char b0f; } E;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern E **DAT_8007b908[];
extern short DAT_80078790[];
extern void FUN_8004cb00(E *, int);
extern int testAreaFlagBit(int);

extern E *APD_DATA[];
void FUN_8004c570(void)
{
    E ***tab;
    E *p;
    unsigned char *q;
    short n;
    int r;
    char pad;
    unsigned b;
    for (p = APD_DATA[DAT_8009c962 & 7]; p->type < 0xff; p++)
        FUN_8004cb00(p, -1);
    n = DAT_80078790[DAT_8009c960];
    tab = (E ***)DAT_8007b908[DAT_8009c960];
    if (n > 15) n = 15;
    p = *tab[n];
    q = &p->type;
    while (*q < 0xff) {
        b = q[0xd] >> 2;
        r = testAreaFlagBit(b);
        if (r == 0) {
            if (p->active == DAT_8009c962 || p->active == ~DAT_8009c962)
                FUN_8004cb00(p, b);
        }
        q += 16;
        p++;
    }
}
#endif
