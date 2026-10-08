// FUNC 80055fac 1024 MAIN0
// MATCHING 80055fac 1024
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} Sprt;
typedef struct { short n, pad, x, y; } Hdr;
typedef struct { unsigned short f, clut, x, y; } Ent;
typedef struct { unsigned char u, v, w, h, p4, flag, p6, p7, p8, p9; } Img;
typedef struct O { char p0[0x1e]; short v1e; char p20[2]; short v22; char p24[0x92]; unsigned short m; char pb8[6];
    unsigned short a, b, c, d; char pc6[4]; unsigned short idx; } O;
typedef struct L { short x0, x1, x2, x3; short a, b, c, d; short m; short pad[7]; } L;

extern void *D_1f800164;
extern int D_1f8001e0;
extern Hdr DAT_800a4648[];
extern Ent DAT_800a4de0[][128];
extern Img DAT_800a5de4[];
extern void FUN_8005f990(char *);
extern void FUN_80060008(void *, int, int, int, int);
extern void FUN_8005e580(int, void *);
extern void FUN_8005e74c(Sprt *);
extern int FUN_8005efe4(void);
extern void FUN_80056420(L *, int, int);

void FUN_80055fac(O *o)
{
    char env[0x60];
    L l;
    void *r;
    int i;
    unsigned short f;
    int j;
    Sprt *s;

    FUN_8005f990(env);
    r = D_1f800164;
    FUN_80060008(r, 0, 0, *(unsigned short *)(env + 0x14), 0);
    FUN_8005e580(D_1f8001e0 + 8, r);
    D_1f800164 = (char *)D_1f800164 + 0xc;
    s = (Sprt *)0x1f800000;
    for (i = 0, j = 0; i < DAT_800a4648[o->idx].n; j++, i++) {
        f = DAT_800a4de0[o->idx][j].f;
        if (*(short *)&DAT_800a4de0[o->idx][j].f & 0x8000) {
            DAT_800a4de0[o->idx][j].f = f - (f & 0x8000);
        } else if (!(f & 0x4000) || o->v22 < 0x1f) {
            FUN_8005e74c(s);
            s->r0 = 0x80;
            s->g0 = 0x80;
            s->b0 = 0x80;
            if (DAT_800a5de4[f &= 0xfff].flag == 0) {
                s->x0 = DAT_800a4648[o->idx].x + DAT_800a4de0[o->idx][j].x;
                s->y0 = DAT_800a4648[o->idx].y + DAT_800a4de0[o->idx][j].y;
            } else {
                s->x0 = DAT_800a4648[o->idx].x + DAT_800a4de0[o->idx][j].x - 7;
                s->y0 = DAT_800a4648[o->idx].y + DAT_800a4de0[o->idx][j].y - 9;
            }
            s->u0 = DAT_800a5de4[(short)f].u << 2;
            s->v0 = DAT_800a5de4[(short)f].v;
            s->w = DAT_800a5de4[(short)f].w;
            s->h = DAT_800a5de4[(short)f].h;
            s->clut = DAT_800a4de0[o->idx][j].clut;
            {
                Sprt *q = D_1f800164;
                *q = *s;
                FUN_8005e580(D_1f8001e0 + 8, q);
            }
            D_1f800164 = (char *)D_1f800164 + 0x14;
        }
    }
    if (FUN_8005efe4() != 1)
        FUN_8005efe4();
    r = D_1f800164;
    FUN_80060008(r, 0, 0, 0, 0);
    FUN_8005e580(D_1f8001e0 + 8, r);
    l.m = o->m;
    l.a = o->a;
    l.b = o->b;
    l.c = o->c;
    l.d = o->d;
    D_1f800164 = (char *)D_1f800164 + 0xc;
    FUN_80056420(&l, 0, o->v1e);
}
