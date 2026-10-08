// FUNC 8011bb24 216 X006
// MATCHING 8011bb24 216
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
} SP8;
extern SP8 *D_1F800164;
extern char *D_1F8001E0;
extern void SetSprt8(SP8 *);
extern void SetSemiTrans(SP8 *, int);
extern unsigned short GetClut(int, int);
extern void AddPrim(void *, void *);

void func_8011BB24(short x, short y, unsigned char u, unsigned char v, short c)
{
    SP8 *p = D_1F800164;
    SetSprt8(p);
    p->code |= 1;
    SetSemiTrans(p, 0);
    p->x0 = x;
    p->y0 = y;
    p->u0 = u;
    p->v0 = v;
    p->clut = GetClut(0x170, c + 0x1f0);
    AddPrim(D_1F8001E0 + 4, p);
    D_1F800164 = (SP8 *)((char *)D_1F800164 + 0x10);
}
