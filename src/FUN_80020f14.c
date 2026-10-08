// FUNC 80020f14 572 MAIN0
// MATCHING 80020f14 572
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
} PTAG;
typedef struct {
    PTAG tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
} SPRT8;
typedef struct {
    PTAG tag;
    int code[2];
} DRM;
extern char *DAT_8009d540;
extern PTAG *DAT_1f8001e0;
extern short DAT_1f8001f4;
extern char DAT_800a2c90[];
extern char DAT_800a2ca0[];
extern unsigned short FUN_8005e420(int, int);
extern void FUN_80060008(DRM *p, int dfe, int dtd, int tpage, void *tw);

void FUN_80020f14(short x, short y, int c, unsigned char *s)
{
    SPRT8 sp;
    SPRT8 *q;
    SPRT8 *p;
    DRM *d;
    q = &sp;
    while (*s) {
        if (!(DAT_8009d540 < DAT_800a2c90 + DAT_1f8001f4 * 0x780))
            break;
        q->code = 0x75;
        q->tag.len = 3;
        q->r0 = 0x80;
        q->g0 = 0x80;
        q->b0 = 0x80;
        q->x0 = x;
        q->y0 = y;
        q->code &= ~2;
        q->u0 = (*s & 0xf) << 3;
        q->v0 = (*s >> 4) << 3;
        q->clut = FUN_8005e420(0x170, (short)c + 0x1f0);
        p = (SPRT8 *)DAT_8009d540;
        *p = *q;
        p->tag.addr = DAT_1f8001e0->addr;
        DAT_1f8001e0->addr = (unsigned)p;
        DAT_8009d540 += 0x10;
        s++;
        x += 8;
    }
    d = (DRM *)DAT_8009d540;
    if ((char *)d < DAT_800a2ca0 + DAT_1f8001f4 * 0x780) {
        FUN_80060008(d, 0, 0, 0x15, 0);
        d->tag.addr = DAT_1f8001e0->addr;
        DAT_1f8001e0->addr = (unsigned)d;
        DAT_8009d540 += 0xc;
    }
}
