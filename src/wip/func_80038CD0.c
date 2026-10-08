// FUNC 80038cd0 1152 MAIN0
// Score 10: only case 2 differs: game keeps p[1] in a1 and loads vars[p[2]] into v1 before computing the p[1] address; ours reuses v1 for p[1]. b18 tried: i1/i2/v temps in every order and type (int/short/uchar/ushort), function-scope i/r as temps, inline get/set helpers, q = p+1 forms, p = D_8009D60C + pc forms: all stay 10 (local-alloc ties p[1] to the dying p register).
typedef struct {
    unsigned short lab[0x45];
    unsigned short pc;   /* 0x8a */
    unsigned short sp;   /* 0x8c */
    short pad8e;
    int stack[0x400];    /* 0x90 */
    int vars[64];        /* 0x1090 */
    int x1190[16];
    int x11D0;
    int x11D4;
} G;
typedef struct {
    char pad[0x88];
    unsigned char b88;
    char b89;
    unsigned short pc;
} H;

extern G *D_8009F0F0;
extern unsigned char *D_8009D60C;
extern int D_8009F2D8[64];
extern int Rand(void);
extern void FUN_800382fc(void);
extern void FUN_8003840c(unsigned char);
extern void func_800386F4(int);
extern void FUN_800387e0(unsigned char);
extern void func_800388D4(unsigned char);

static __inline__ void push(G *g, int v)
{
    g->stack[g->sp] = v;
    g->sp++;
}

static __inline__ int pop(G *g)
{
    return g->stack[--g->sp];
}

static __inline__ void clear(void)
{
    int i;
    for (i = 63; i >= 0; i--) {
        D_8009F2D8[i] = 0;
    }
}

int func_80038CD0(unsigned char op)
{
    H *h = (H *)D_8009F0F0;
    int i;
    int r;
    char buf[4];

    switch (op) {
    case 1:
        clear();
        r = 0;
        goto reset;
    case 2:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        p = (unsigned char *)(g->pc + (int)D_8009D60C);
        a = g->vars[p[2]];
        g->vars[p[1]] = a;
        g->pc += 3;
        r = 1;
        }
        break;
    case 3:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        p = (unsigned char *)(g->pc + (int)D_8009D60C);
        q = p + 2;
        k = p[1];
        for (a = 0; a < 4; a++) {
            buf[a] = q[a];
        }
        g->vars[k] = *(int *)buf;
        g->pc += 6;
        r = 1;
        }
        break;
    case 4:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        p = (unsigned char *)(g->pc + (int)D_8009D60C);
        {
        int i1 = p[1], i2 = p[2];
        b = g->vars[i1];
        a = g->vars[i2];
        g->vars[i1] = a;
        g->vars[i2] = b;
        }
        g->pc += 3;
        r = 1;
        }
        break;
    case 6:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        p = (unsigned char *)(g->pc + (int)D_8009D60C);
        k = p[1];
        g->vars[k] = Rand() & 0xffff;
        g->pc += 2;
        r = 1;
        }
        break;
    case 5:
        FUN_800382fc();
        r = 1;
        break;
    case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16:
        FUN_8003840c(op);
        r = 1;
        break;
    case 17:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        p = (unsigned char *)(g->pc + (int)D_8009D60C);
        if (--g->vars[p[1]] > 0) {
            q = (unsigned char *)(g->pc + (int)D_8009D60C) + 2;
            for (a = 0; a < 2; a++) {
                buf[a] = q[a];
            }
            g->pc = *(unsigned short *)buf;
        } else {
            g->pc += 4;
        }
        r = 1;
        }
        break;
    case 18:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        q = D_8009D60C;
        a = g->pc + 2;
        g->stack[g->sp] = a;
        g->sp = g->sp + 1;
        g->pc = g->lab[q[g->pc + 1]] - 1;
        r = 1;
        }
        break;
    case 19:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        g->pc = *(unsigned short *)&g->stack[--g->sp];
        r = 1;
        }
        break;
    case 20:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        {
        G *h = g;
        for (k = 0; k < 64; k++) {
            push(h, g->vars[k]);
        }
        }
        g->pc += 1;
        r = 1;
        }
        break;
    case 21:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        G *h = g;
        for (k = 63; k >= 0; k--) {
            g->vars[k] = pop(h);
        }
        g->pc += 1;
        r = 1;
        }
        break;
    case 22:
        func_800386F4(0);
        r = 1;
        break;
    case 23:
        func_800386F4(1);
        r = 1;
        break;
    case 24: case 25: case 26: case 27:
        FUN_800387e0(op);
        r = 1;
        break;
    case 28: case 29: case 30: case 31: case 32: case 33: case 34: case 35:
    case 36: case 37: case 38: case 39: case 40: case 41: case 42: case 43:
        func_800388D4(op);
        r = 1;
        break;
    case 44:
        {
        G *g = D_8009F0F0;
        unsigned char *p, *q;
        char *d;
        int a, b, k;
        p = (unsigned char *)(g->pc + (int)D_8009D60C);
        r = 0;
        k = p[1];
        g->x11D0 = 0;
        ((H *)g)->b88 = 2;
        g->pc += 2;
        g->x11D4 = k;
        }
        break;
    default:
        clear();
        r = 0;
    reset:
        h->b88 = 0;
        h->pc = 0;
        break;
    }
    return r;
}
