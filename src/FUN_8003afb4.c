// FUNC 8003afb4 2888 MAIN0
// MATCHING 8003afb4 2888
#include "TOBJ.H"
typedef struct { char pad[0x8a]; unsigned short w8a; char pad2[0x1190 - 0x8c]; int d1190; int d1194; int d1198; int d119c; } G;
typedef struct { short s0, s2; } S2;
extern G *D_8009F0F0;
extern TObj *D_8009F2D8[];
extern S2 *D_800A6078, *D_800A607C[];
extern short D_800A604C[];
extern unsigned char D_8009CDA4[], D_8009CEA4[];
extern signed char D_8009D2B0;
extern unsigned char D_800A60A0, D_800A60A3, D_800A60D6, D_800A60A1, D_8009D2B1, D_800A611A;
extern unsigned char D_8009C93A, D_8009C942, D_8009C975, D_8009C93C, D_8009C976;
extern short D_800A6066;
extern int D_8009C96C;
extern char D_800A6038[];
extern short D_800A60D0[];
extern unsigned char D_8009C971;
extern short D_800A60D2[];
extern unsigned char D_8009C970[];
extern void FUN_800399c4(void);
extern void FUN_80039e54(void);
extern void FUN_8003a89c(void);
extern void func_80113754(TObj *);
extern void func_800ECBBC(TObj *);
extern void func_800E8348(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_800ecb40(char *, int, int);
extern void FUN_8005a7a0(int, int, int);
extern void func_8004D620(int, int);
extern void FUN_8001e688(int, int, int);
extern void func_800EDFB8(TObj *, int);
extern void playSFX(int);
extern void FUN_80026f4c(void);
extern void addItemToInventory(int, int, int);
extern void FUN_800eeae4(char *, int, int);

static __inline__ int op81(void)
{
    G *g = D_8009F0F0;
    TObj *o;
    int b, a;
    a = g->d1194;
    o = D_8009F2D8[g->d1190];
    b = g->d1198;
    if (o != 0) {
        o->w74 = a;
        o->w76 = b;
        switch (o->type & 0x7f) {
        case 0x18:
            func_80113754(o);
            break;
        case 0x19:
            func_800ECBBC(o);
            break;
        case 0x2e:
            func_800E8348(o);
            break;
        }
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op82(void)
{
    G *g = D_8009F0F0;
    TObj **p = &D_8009F2D8[g->d1190];
    TObj *o = *p;
    if (o != 0) {
        o->b04 = 3;
        if (o->d94 != 0) ((TObj *)o->d94)->b04 = 3;
        *p = 0;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op83(void)
{
    G *g = D_8009F0F0;
    D_800A6078->s2 = g->d1190;
    D_800A604C[1] = g->d1194;
    D_800A607C[0]->s2 = g->d1198;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op84(void)
{
    G *g = D_8009F0F0;
    int v;
    g->d1190 = D_800A6078->s2;
    g->d1194 = D_800A604C[1];
    v = D_800A607C[0]->s2;
    (*(short *)((char *)g + 0x8a))++;
    g->d1198 = v;
    return 1;
}

static __inline__ int op86(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) g->d1190 = o->b6a;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op87(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) {
        o->h->p.whole = g->d1194;
        o->y.p.whole = g->d1198;
        o->d->p.whole = g->d119c;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op88(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) {
        g->d1190 = o->h->p.whole;
        g->d1194 = o->y.p.whole;
        g->d1198 = o->d->p.whole;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op89(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) o->animFrame = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8a(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) g->d1190 = o->animFrame;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8b(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) g->d1190 = o->b04 == 2;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8c(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) o->active = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8d(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) {
        FUN_800202b4(o);
        g->d1190 = o->visible;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op8e(void)
{
    G *g = D_8009F0F0;
    int v = D_8009CDA4[g->d1190];
    (*(short *)((char *)g + 0x8a))++;
    g->d1190 = v;
    return 1;
}

static __inline__ int op8f(void)
{
    G *g = D_8009F0F0;
    D_8009CDA4[g->d1190] = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op90(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) {
        int t = o->type & 0x7f;
        if (t == 0x18) goto l;
        if (t == 0x19) {
        l:
            g->d1190 = o->b68;
        }
    } else {
        g->d1190 = 0;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op91(void)
{
    G *g = D_8009F0F0;
    int v = D_8009CEA4[g->d1190];
    (*(short *)((char *)g + 0x8a))++;
    g->d1190 = v;
    return 1;
}

static __inline__ int op92(void)
{
    G *g = D_8009F0F0;
    D_8009CEA4[g->d1190] = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op93(void)
{
    G *g = D_8009F0F0;
    if (D_8009D2B0 != 3) FUN_800ecb40(D_800A6038, D_800A6078->s2, D_800A604C[1]);
    g->d1190 = D_800A60A0;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op94(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if ((D_8009D2B0 != 3 || D_800A60A3 == 1) && o != 0) o->b6b = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op95(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) {
        switch (o->type & 0x7f) {
        case 0x18:
            o->b0f = g->d1194;
            break;
        case 0x19:
        case 0x2e:
            {
                int c = g->d1194;
                ((unsigned char *)o)[0xe] = c;
                switch (c & 0xff) {
                case 1: *(signed char *)&o->b0f = -11; break;
                case 2: o->b0f = 0x10; break;
                }
            }
            break;
        }
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op96(void)
{
    G *g = D_8009F0F0;
    if (D_8009D2B0 != 3) {
        switch (g->d1190) {
        case 0: D_800A60A3 = 0; break;
        case 1: D_800A60A3 = 1; break;
        default: D_800A60A3 = 2; break;
        }
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op98(void)
{
    char *p = (char *)D_8009F0F0;
    *(int *)(p + 0x1190) = D_800A60D6;
    *(short *)(p + 0x8a) += 1;
    return 1;
}

static __inline__ int op99(void)
{
    G *g = D_8009F0F0;
    D_800A6066 = g->d1190;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9a(void)
{
    G *g = D_8009F0F0;
    {
        int a = g->d1190;
        if (g->d1194) FUN_8005a7a0(a, 1, 0);
        else FUN_8005a7a0(a, 0, 0);
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9b(void)
{
    unsigned char *p = (unsigned char *)D_8009F0F0;
    *(int *)(p + 0x1190) = D_800A60A1;
    (*(short *)(p + 0x8a))++;
    return 1;
}

static __inline__ int op9c(void)
{
    G *g = D_8009F0F0;
    D_8009C942 = g->d1190;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9d(void)
{
    G *g = D_8009F0F0;
    func_8004D620(g->d1190, 2);
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9e(void)
{
    G *g = D_8009F0F0;
    FUN_8001e688(g->d1190, g->d1194, g->d1198);
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int op9f(void)
{
    unsigned char *g = (unsigned char *)D_8009F0F0;
    unsigned char *p = (unsigned char *)D_8009F2D8[*(int *)(g + 0x1190)];
    int f = *(int *)(g + 0x1194);
    if (p != 0 && p[2] == 0x19) {
        if (f == 0) {
            (*(unsigned char **)(p + 0x94))[4] = 3;
            *(int *)(p + 0x94) = 0;
        } else {
            func_800EDFB8((TObj *)p, 1);
        }
    }
    (*(short *)(g + 0x8a))++;
    return 1;
}

static __inline__ int opa0(void)
{
    G *g = D_8009F0F0;
    D_8009C975 = 0x10;
    D_8009C93C = g->d1190;
    D_8009C976 = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa2(void)
{
    char *p = (char *)D_8009F0F0;
    int a = *(int *)(p + 0x1190);
    int b = *(int *)(p + 0x1194);
    int c = *(int *)(p + 0x1198);
    if (a < 0) {
        if (c != 0) {
            func_8004D620(0x15, 3);
            playSFX(10);
        }
        FUN_80026f4c();
    } else {
        addItemToInventory(a, b, c);
    }
    *(unsigned short *)(p + 0x8a) += 1;
    return 1;
}

static __inline__ int opa3(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) g->d1190 = o->b69;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa4(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) o->b69 = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa5(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->d1190];
    if (o != 0) o->b9c = g->d1194;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa6(void)
{
    G *g = D_8009F0F0;
    FUN_800eeae4(D_800A6038, (short)g->d1190, (short)g->d1194);
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa7(void)
{
    G *g = D_8009F0F0;
    g->d1190 = D_8009D2B1;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opa8(void)
{
    char *p = (char *)D_8009F0F0;
    *(int *)(p + 0x1190) = D_800A611A;
    *(unsigned short *)(p + 0x8a) += 1;
    return 1;
}

static __inline__ int opa9(void)
{
    G *g = D_8009F0F0;
    unsigned char m = D_8009C971;
    short v = D_800A60D0[0];
    int a = g->d1190;
    if (v < m) {
        D_800A60D0[0] = v + a;
        if (D_800A60D0[0] > m) D_800A60D0[0] = m;
        D_800A60D2[0] = D_800A60D0[0];
        D_8009C970[0] = D_800A60D0[0];
        g->d1190 = 0;
    } else {
        g->d1190 = 1;
    }
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opaa(void)
{
    G *g = D_8009F0F0;
    g->d1190 = D_8009C96C;
    (*(short *)((char *)g + 0x8a))++;
    return 1;
}

static __inline__ int opab(void)
{
    G *g = D_8009F0F0;
    (*(short *)((char *)g + 0x8a))++;
    return 0;
}

static __inline__ int op97(void)
{
    if (D_8009C93A == 0) return 0;
    FUN_8003a89c();
    return 1;
}

int FUN_8003afb4(unsigned char op)
{
    int r = 0;

    switch (op) {
    case 0xab:
        r = opab();
        break;
    case 0x80: FUN_800399c4(); r = 1; break;
    case 0x81: r = op81(); break;
    case 0x82: r = op82(); break;
    case 0x83: r = op83(); break;
    case 0x84: r = op84(); break;
    case 0x85: FUN_80039e54(); r = 1; break;
    case 0x86: r = op86(); break;
    case 0x87: r = op87(); break;
    case 0x88: r = op88(); break;
    case 0x89: r = op89(); break;
    case 0x8a: r = op8a(); break;
    case 0x8b: r = op8b(); break;
    case 0x8c: r = op8c(); break;
    case 0x8d: r = op8d(); break;
    case 0x8f: r = op8f(); break;
    case 0x8e: r = op8e(); break;
    case 0x90: r = op90(); break;
    case 0x91: r = op91(); break;
    case 0x92: r = op92(); break;
    case 0x93: r = op93(); break;
    case 0x94: r = op94(); break;
    case 0x95: r = op95(); break;
    case 0x96: r = op96(); break;
    case 0x98: r = op98(); break;
    case 0x99: r = op99(); break;
    case 0x9a: r = op9a(); break;
    case 0x9b: r = op9b(); break;
    case 0x9c: r = op9c(); break;
    case 0x9d: r = op9d(); break;
    case 0x9e: r = op9e(); break;
    case 0x9f: r = op9f(); break;
    case 0xa0: r = opa0(); break;
    case 0xa1: r = op9d(); break;
    case 0xa2: r = opa2(); break;
    case 0xa3: r = opa3(); break;
    case 0xa4: r = opa4(); break;
    case 0xa5: r = opa5(); break;
    case 0xa6: r = opa6(); break;
    case 0xa7: r = opa7(); break;
    case 0xa8: r = opa8(); break;
    case 0xa9: r = opa9(); break;
    case 0xaa: r = opaa(); break;
    case 0x97:
        r = op97();
        break;
    }
    return r;
}
