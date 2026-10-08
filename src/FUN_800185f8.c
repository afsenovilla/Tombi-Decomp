// FUNC 800185f8 128 MAIN0
// MATCHING 800185f8 128
typedef struct Q { char pad[0x40]; char *a; char *b; } Q;
extern short DAT_a;
extern Q **DAT_b;
extern unsigned short DAT_c;
Q *FUN_800185f8(void)
{
    Q *q; short n = DAT_a;
    if (n > 0) {
        DAT_a = n - 1; q = *DAT_b++;
        if ((DAT_c & 1) == 0) { q->a = (char *)q + 0x10; q->b = (char *)q + 0x18; }
        else { q->b = (char *)q + 0x10; q->a = (char *)q + 0x18; }
        return q;
    }
    return 0;
}
