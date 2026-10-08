// FUNC 80058b18 528 MAIN0
// MATCHING 80058b18 528
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct {
    char pad[0x18];
    unsigned char *base;
    short **tbl;
} SPR;
extern unsigned char DAT_8009c971;
extern SPRT *DAT_1f800164;
extern char *DAT_1f8001e0;
extern unsigned short *PTR_DAT_800120a8[];
extern void SetSprt(SPRT *p);
extern void SetSemiTrans(SPRT *p, int abe);
extern unsigned short GetClut(int x, int y);
extern void AddPrim(void *ot, void *p);
extern void FUN_800595f4(SPR *s, int x, int y, int v);
extern void AddDrawMode(int a, int b);

void FUN_80058b18(SPR *s, short x, short y, unsigned short n)
{
    int i = 0;
    SPRT *p;
    unsigned char *e;
    int cy;

    for (; i < DAT_8009c971; i++) {
        {
            p = DAT_1f800164;
            e = s->base + *(short *)(*(unsigned char *volatile *)&s->base + (*s->tbl[i] << 2) + 2);
            SetSprt(p);
            p->code |= 1;
            SetSemiTrans(p, 0);
            p->x0 = x + (signed char)e[0xe];
            p->y0 = y + (signed char)e[0xf];
            p->u0 = e[0];
            p->v0 = e[1];
            p->w = e[10];
            p->h = e[11];
            if (i < (short)n) {
                if ((short)n < 3)
                    p->clut = GetClut(0x170, 0x1f7);
                else if ((short)n < 5)
                    p->clut = GetClut(0x170, 0x1f6);
                else if ((short)n < 7)
                    p->clut = GetClut(0x170, 0x1f5);
                else
                    p->clut = GetClut(0x170, 0x1f4);
            } else {
                p->clut = GetClut(0x170, 0x1f8);
            }
            AddPrim(DAT_1f8001e0 + 4, p);
            DAT_1f800164 = DAT_1f800164 + 1;
        }
    }
    FUN_800595f4(s, (short)x, (short)y, *PTR_DAT_800120a8[(short)n]);
    AddDrawMode(0x15, 1);
}
