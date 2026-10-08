// FUNC 80039580 1092 MAIN0
/* score 275 (b30, was 291): case ranges (`case 0 ... 2:` gives the game's slti/bltz), and the tail written as
   x = p->b88; if (x == 1) { loop; x = p->b88; } D_8009D4F0 = x; (game stores the compare value on the skip path).
   Left: game cross-jumps copies in pairs (case0 tail into case1, case2 into case0x13) and allocates x differently per pair
   (pair 1: x=v0 with `move v0,v1` in the bne slot; pair 2: x=v1, constant 2 kept in a0 from the switch); ours merges every
   copy into the last one. Earlier: head of each inline copy matches (D_8009F0F0 = &D_8009F840; p = D_8009F0F0). */
typedef struct { char p0[0x84]; unsigned char *p84; unsigned char b88; char p89; unsigned short w8a; } S;
extern int D_8009D69C;
extern unsigned short D_800A603C;
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern S D_8009F840;
extern S *D_8009F0F0;
extern unsigned char *D_8009D60C;
extern unsigned int D_800A0A10, D_800A0A14;
extern int D_8009D4F0;
extern int func_80038CD0(unsigned char);
extern int func_8003AFB4(unsigned char);

static __inline__ void run(void)
{
    S *p;
    int r;
    unsigned char x;
    D_8009F0F0 = &D_8009F840;
    p = D_8009F0F0;
    D_8009D60C = D_8009F840.p84;
    if (D_8009F840.b88 == 2 && ++D_800A0A10 >= D_800A0A14) {
        D_8009F840.b88 = 1;
    }
    x = p->b88;
    if (x == 1) {
        do {
            unsigned char c = D_8009D60C[D_8009F0F0->w8a];
            if (c < 0x80) {
                r = func_80038CD0(c);
            } else {
                r = func_8003AFB4(c);
            }
        } while (r != 0);
        x = p->b88;
    }
    D_8009D4F0 = x;
}

void func_80039580(void)
{
    if (D_8009D69C == 0) return;
    if (D_800A603C == 0x505) return;
    switch (D_8009C960) {
    case 0:
        switch (D_8009C962) {
        case 0 ... 2:
            run();
        }
        break;
    case 1:
        switch (D_8009C962) {
        case 0 ... 4:
            run();
        }
        break;
    case 2:
        switch (D_8009C962) {
        case 0 ... 2:
            run();
        }
        break;
    case 0x13:
        switch (D_8009C962) {
        case 0 ... 1:
            run();
        }
        break;
    }
}
