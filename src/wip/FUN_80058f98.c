// FUNC 80058f98 552 MAIN0
typedef struct SPRT { unsigned int tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut; short w, h; } SPRT;
extern unsigned char DAT_8009d2b2;
extern SPRT *DAT_1f800164;
extern int DAT_1f8001e0;
extern unsigned short *PTR_DAT_800121b8, *PTR_DAT_800121cc, *PTR_DAT_800121d0, *PTR_DAT_800121d4, *PTR_DAT_800121b0, *PTR_DAT_800121b4;
extern void FUN_800595f4(int, short, short, unsigned short);
extern void FUN_80059464(int, int);
extern void SetSprt(SPRT *);
extern void SetSemiTrans(SPRT *, int);
extern void AddPrim(void *, void *);

void FUN_80058f98(int o, int x, int y)
{
    SPRT *p;
    unsigned char *q;
    int m;
    void *ot;
    unsigned char c;
    switch (DAT_8009d2b2) {
    case 0:
        FUN_800595f4(o, x, y, *PTR_DAT_800121b8);
        FUN_80059464(0x14, 1);
        break;
    case 5:
        FUN_800595f4(o, x, y, *PTR_DAT_800121cc);
        FUN_80059464(0x15, 1);
        break;
    case 6:
        FUN_800595f4(o, x, y, *PTR_DAT_800121d0);
        FUN_80059464(0x15, 1);
        break;
    case 7:
        FUN_800595f4(o, x, y, *PTR_DAT_800121d4);
        FUN_80059464(0x15, 1);
        break;
    case 8:
        FUN_800595f4(o, x, y, *PTR_DAT_800121b0);
        FUN_80059464(0x14, 1);
        break;
    case 9:
        p = DAT_1f800164;
        q = (unsigned char *)(*(volatile int *)(o + 0x18) + *(short *)(*(volatile int *)(o + 0x18) + *PTR_DAT_800121b4 * 4 + 2));
        SetSprt(p);
        p->code |= 1;
        SetSemiTrans(p, 0);
        p->x0 = (signed char)q[0xe] + x;
        p->y0 = (signed char)q[0xf] + y;
        p->u0 = q[0];
        p->v0 = q[1];
        ot = (void *)(DAT_1f8001e0 + 4);
        p->w = q[0xa];
        c = q[0xb];
        p->clut = 0x7e14;
        p->h = c;
        AddPrim(ot, p);
        DAT_1f800164++;
        FUN_80059464(0x14, 1);
        break;
    }
}
