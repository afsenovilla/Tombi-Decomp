// FUNC 80047af0 900 MAIN0
// MATCHING 80047af0 900
typedef struct {
    unsigned char b0; char p1[3]; unsigned char b4, b5, b6;
    char p7[0x16 - 7]; short s16;
    char p18[0x9e - 0x18]; unsigned char b9e;
} TO;
typedef struct { char p[0x4c]; unsigned short s4c; } C;
extern unsigned char D_1F8001A5, D_1F8001A4;
extern int D_8009C960;
extern unsigned short D_8009C960_h;
extern unsigned short D_8009C962;
extern short *D_800A605C;
extern unsigned short D_80115320[];
extern unsigned short D_800A60A4, D_800A60A6, D_800A60A8, D_800A60AA;
extern TO D_800A6038;
extern unsigned char D_8009C938, D_8009C942, D_8009C93F;
extern signed char D_8009D2B0;
extern unsigned short D_1F800248;
extern short D_1F80019E;
extern unsigned char **D_1F800228;
extern C *D_1F8001D4;
extern short D_800A4582;
extern void func_8011E514(void), FUN_800490ac(void), func_80048E68(void), func_800491F0(void);
extern void func_80046D58(TO *), FUN_8004432c(TO *, unsigned char *), func_80047484(TO *), func_80126770(TO *);
extern void func_80045F00(void), FUN_80045d98(void), func_80046610(void), func_80049524(void);

static __inline__ void collide(TO *p)
{
    unsigned char **q;
    unsigned char *e;
    if (D_8009C938 == 0 && D_8009D2B0 != 3) {
        q = D_1F800228;
        for (D_1F80019E = D_1F800248; D_1F80019E != 0;) {
            e = *q;
            D_1F80019E--;
            q++;
            if (*e & 3) FUN_8004432c(p, e);
        }
    }
}

void func_80047AF0(void)
{
    unsigned short *t;
    TO *p;

    if (D_1F8001A5 != 0) return;
    if (D_8009C960 == 6) {
        func_8011E514();
        return;
    }
    t = &D_80115320[D_800A605C[1] * 4];
    D_800A60A4 = *t++;
    D_800A60A6 = *t++;
    D_800A60A8 = *t++;
    D_800A60AA = *t++;
    FUN_800490ac();
    func_80048E68();
    func_800491F0();
    p = &D_800A6038;
    switch (p->b0) {
    case 0: case 5:
    default:
        goto dflt;
    case 1: case 2: case 3: case 7:
        switch (p->b9e) {
        case 0: case 5: case 6:
            func_80046D58(p);
        }
        collide(p);
        if (D_8009C942 | D_8009C93F) goto tail;
        func_80047484(p);
        break;
    case 4: case 6:
        collide(p);
        if (D_8009C942 | D_8009C93F) goto tail;
        break;
    }
    if (D_1F8001A4 == 0 && D_1F8001D4->s4c == 1 && D_8009D2B0 != 3 && D_800A4582 + 0xa0 < p->s16) {
        p->b0 = 5;
        p->b4 = 2;
        p->b5 = 3;
        p->b6 = 0;
    }
dflt:
    if (!(D_8009C942 | D_8009C93F) && D_8009C960_h == 0 && D_8009C962 == 2)
        func_80126770(p);
tail:
    func_80045F00();
    FUN_80045d98();
    func_80046610();
    func_80049524();
}
