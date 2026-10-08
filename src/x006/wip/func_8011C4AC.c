// FUNC 8011c4ac 1900 X006
/* score 642: timer HUD (11 SPRT_8 + h:mm:ss.cc digits), whole function incl. csv pieces 8011C5CC/8011C758/8011CA00. Code shape right; register allocation differs (game keeps 0x20/0x28 and the /10, /60 magic constants in s-regs, frame 0x40 with a spill at 0x10(sp)). */
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

#define SPR(p, x, y, u, v) \
    SetSprt8(p); \
    (p)->code |= 1; \
    SetSemiTrans(p, 0); \
    (p)->x0 = (x); \
    (p)->y0 = (y); \
    (p)->u0 = (u); \
    (p)->v0 = (v); \
    (p)->clut = GetClut(0x170, 0x1f0); \
    AddPrim((char *)D_1F8001E0 + 4, p);

#define NEXT(p) \
    p = (SPRT_8 *)D_1F800164 + 1; \
    D_1F800164 = (char *)p;

void func_8011C4AC(int unused, short x, short y)
{
    short n;
    int a;
    int b;
    short d0;
    short d1;
    SPRT_8 *p;






    p = (SPRT_8 *)D_1F800164;
    SPR(p, x, y, 0x10, 0x20);
    NEXT(p);
    SPR(p, x + 8, y, 0x28, 0x20);
    NEXT(p);
    SPR(p, x + 0x10, y, 0x18, 0x28);
    NEXT(p);
    SPR(p, x + 0x18, y, 0x20, 0x28);
    NEXT(p);
    SPR(p, x + 0x30, y, 0x38, 0x10);
    NEXT(p);
    SPR(p, x + 0x48, y, 0x10, 0x10);

    n = D_8009CFD0 / 3600;
    d0 = n % 10;
    NEXT(p);
    SPR(p, x + 0x28, y, d0 << 3, 0x18);

    n = D_8009CFD0 / 60;
    a = n % 60;
    b = a / 10;
    d0 = b % 10;
    d1 = a - b * 10;
    NEXT(p);
    SPR(p, x + 0x38, y, d0 << 3, 0x18);
    NEXT(p);
    SPR(p, x + 0x40, y, d1 << 3, 0x18);

    n = D_8009CFD0 % 60;
    a = n * 100 / 60;
    b = a / 10;
    d0 = b % 10;
    d1 = a - b * 10;
    NEXT(p);
    SPR(p, x + 0x50, y, d0 << 3, 0x18);
    NEXT(p);
    SPR(p, x + 0x58, y, d1 << 3, 0x18);

    D_1F800164 += 0x10;
    AddDrawMode(0x15, 1);
}
