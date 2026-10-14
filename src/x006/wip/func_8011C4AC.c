// FUNC 8011c4ac 1900 X006
/* score 141 (was 642): timer HUD (11 SPRT_8 + h:mm:ss.cc digits), whole function incl. csv pieces 8011C5CC/8011C758/
   8011CA00. Keys: inline spr() with unsigned char u/v (args evaluated before the calls, constant CSE as game), fresh
   D_1F800164 pointer per sprite, digit2() inline taking both digits (x+0x40 computed early, x+0x38 spilled).
   Left: d1 (= a % 10) should be zero-extended at its use (game sll 16/srl 13 after the calls, computed into a's reg
   just before the first jal); ours sign-extends/narrows it at binding, so the 0x88888889 constant gets the spill. */
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
} SPRT_8;

extern short D_8009CFD0;
extern char *D_1F800164;
extern int D_1F8001E0;
extern void SetSprt8(SPRT_8 *);
extern void SetSemiTrans(void *, int);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);
extern void AddDrawMode(int, int);

static __inline__ void spr(SPRT_8 *p, short x, short y, unsigned char u, unsigned char v)
{
    SetSprt8(p);
    p->code |= 1;
    SetSemiTrans(p, 0);
    p->x0 = x;
    p->y0 = y;
    p->u0 = u;
    p->v0 = v;
    p->clut = GetClut(0x170, 0x1f0);
    AddPrim((char *)D_1F8001E0 + 4, p);
}
#define SPR(p, x, y, u, v) spr(p, x, y, u, v)
#define SPRN(x, y, u, v) { SPRT_8 *q = (SPRT_8 *)D_1F800164 + 1; D_1F800164 = (char *)q; spr(q, x, y, u, v); }

static __inline__ void digit(short x, short y, int d)
{
    SPRN(x, y, d << 3, 0x18);
}

static __inline__ void digit2(short xa, short xb, short y, int d0, int d1)
{
    SPRN(xa, y, d0 << 3, 0x18);
    SPRN(xb, y, d1 << 3, 0x18);
}

void func_8011C4AC(int unused, short x, short y)
{
    short n;
    int a;

    SPR((SPRT_8 *)D_1F800164, x, y, 0x10, 0x20);
    SPRN(x + 8, y, 0x28, 0x20);
    SPRN(x + 0x10, y, 0x18, 0x28);
    SPRN(x + 0x18, y, 0x20, 0x28);
    SPRN(x + 0x30, y, 0x38, 0x10);
    SPRN(x + 0x48, y, 0x10, 0x10);

    n = D_8009CFD0 / 3600;
    digit(x + 0x28, y, n % 10);

    n = D_8009CFD0 / 60;
    a = n % 60;
    digit2(x + 0x38, x + 0x40, y, (short)(a / 10) % 10, a % 10);

    n = D_8009CFD0 % 60;
    a = (short)(n * 100 / 60);
    digit2(x + 0x50, x + 0x58, y, (short)(a / 10) % 10, a % 10);

    D_1F800164 += 0x10;
    AddDrawMode(0x15, 1);
}
