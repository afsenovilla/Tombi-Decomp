// FUNC 800e9cb4 492 D0SPSUB
// MATCHING 800e9cb4 492
// FLAGS -O2 -G0 -fno-strength-reduce
typedef struct { unsigned tag; unsigned char r0, g0, b0, code; short x0, y0; unsigned char u0, v0; unsigned short clut; short w, h; } SPRT;
typedef struct { unsigned short clut; unsigned char pad[6]; unsigned char w, h; unsigned char pad2[2]; signed char dx, dy; unsigned char pad3[2]; } T;
typedef struct { char p[8]; unsigned short *frame; } Anim;
typedef struct {
    char p0[0x18];
    char *data;
    char p1c[4];
    Anim *anim;
    unsigned short **digits;
    char p28[0x14];
    unsigned char num[8];
} Obj;
extern SPRT *D_1F800164;
extern int D_1F8001E0;
extern void SetSprt(SPRT *);
extern void SetSemiTrans(SPRT *, int);
extern void AddPrim(void *, SPRT *);
extern void AddDrawMode(int, int);
extern void drawSpriteFrame(void *, int, short, unsigned short);

void subScreenDrawDigitsSprite(Obj *o, short x, short y)
{
    int started = 0;
    unsigned char *d = o->num;
    int i = 0;
    int off = (o->num[0] == 0) * 8;
    short *e;
    int n;
    unsigned char *s;
    T *t;
    SPRT *p;
    do {
        int dx;
        if (i == 7 || *d != 0 || started != 0) {
            dx = off - 0x3C;
            drawSpriteFrame(o, x + dx, y, *o->digits[*d]);
            started++;
            off += 8;
        }
        i++;
        d++;
    } while (i < 8);
    e = (short *)(o->data + *o->anim->frame * 4);
    s = (unsigned char *)(*(char * volatile *)&o->data + e[1]);
    t = (T *)(s + 2);
    n = *e;
    do {
        p = D_1F800164;
        SetSprt(p);
        p->code |= 1;
        SetSemiTrans(p, 1);
        p->x0 = x + t->dx;
        p->y0 = y + t->dy;
        p->u0 = *s;
        n--;
        p->v0 = ((unsigned char *)t)[-1];
        p->w = t->w;
        s += 0x10;
        p->h = t->h;
        p->clut = t->clut;
        AddPrim((void *)(D_1F8001E0 + 4), p);
        D_1F800164 = D_1F800164 + 1;
        t++;
    } while (n != 0);
    AddDrawMode(0x15, 1);
}
