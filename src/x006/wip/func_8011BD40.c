/* score 246 (whole 1900 B incl. csv piece 8011BEAC): the 6 label sprites match exactly; digit part differs in register allocation: game keeps the /10 magic 0x66666667 in s7 and spills only the /60 magic (0x18(sp)); ours spills both (frame +8 in places). Also game computes the u byte as (d<<16)>>13 (ours andi+sll). Tried types of u/v/x params, of t/m/mm/s/c, separate digit inline. */
// FUNC 8011bd40 1900 X006
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
} SPRT_8;

extern SPRT_8 *D_1F800164;
extern int D_1F8001E0;
extern unsigned short D_8009CFE2;
extern void SetSprt8(SPRT_8 *);
extern void SetSemiTrans(SPRT_8 *, int);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);
extern void AddDrawMode(int, int);

#define S(x) ((short)(x))

static __inline__ void spr(short x, short y, int u, int v)
{
    SPRT_8 *p = D_1F800164;

    SetSprt8(p);
    p->code |= 1;
    SetSemiTrans(p, 0);
    p->x0 = x;
    p->y0 = y;
    p->u0 = u;
    p->v0 = v;
    p->clut = GetClut(0x170, 0x1f0);
    AddPrim((void *)(D_1F8001E0 + 4), p);
}

void func_8011BD40(int a, int x, short y)
{
    short t = D_8009CFE2;
    short m, mm, s, c;

    spr(x, y, 0x20, 0x28);
    D_1F800164++;
    spr(x + 8, y, 0x48, 0x20);
    D_1F800164++;
    spr(x + 0x10, y, 0x68, 0x20);
    D_1F800164++;
    spr(x + 0x18, y, 0x28, 0x20);
    D_1F800164++;
    spr(x + 0x30, y, 0x38, 0x10);
    D_1F800164++;
    spr(x + 0x48, y, 0x10, 0x10);
    D_1F800164++;
    spr(x + 0x28, y, (unsigned short)(S(t / 3600) % 10) << 3, 0x18);
    m = t / 60;
    mm = m % 60;
    D_1F800164++;
    spr(x + 0x38, y, (unsigned short)(S(mm / 10) % 10) << 3, 0x18);
    D_1F800164++;
    spr(x + 0x40, y, (unsigned short)(mm % 10) << 3, 0x18);
    s = t % 60;
    c = s * 100 / 60;
    D_1F800164++;
    spr(x + 0x50, y, (unsigned short)(S(c / 10) % 10) << 3, 0x18);
    D_1F800164++;
    spr(x + 0x58, y, (unsigned short)(c % 10) << 3, 0x18);
    D_1F800164++;
    AddDrawMode(0x15, 1);
}
