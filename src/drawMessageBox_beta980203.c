// VERSION BETA_980203 beta980203 8004AB30 1112
#if defined(VERSION_BETA_980203)
// Beta version: the JP branch of drawMessageBox.c as is.
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

extern void *NEXT_PRIM;
extern int CURRENT_OT;
extern Hdr MESSAGE_SLOTS[];
extern Ent MESSAGE_GLYPHS[][128];
#define G MESSAGE_GLYPHS[o->idx][j]
extern Img DAT_800a5de4[];
extern void FUN_8005f990(char *);
extern void FUN_80060008(void *, int, int, int, int);
extern void FUN_8005e580(int, void *);
extern void FUN_8005e74c(Sprt *);
extern int FUN_8005efe4(void);
extern void drawBalloonFrame(L *, int, int);

void func_800492AC(O *o)
{
    char env[0x60];
    L l;
    void *r;
    int i;
    unsigned short f;
    int t;
    short c;
    int j;
    Sprt *s;

    FUN_8005f990(env);
    r = NEXT_PRIM;
    FUN_80060008(r, 0, 0, *(unsigned short *)(env + 0x14), 0);
    FUN_8005e580(CURRENT_OT + 8, r);
    NEXT_PRIM = (char *)NEXT_PRIM + 0xc;
    s = (Sprt *)0x1f800000;
    for (i = 0, j = 0; i < MESSAGE_SLOTS[o->idx].n; j++, i++) {
        f = G.f;
        if (*(short *)&G.f & 0x8000) {
            G.f = f - (f & 0x8000);
        } else if (!(f & 0x4000) || o->v22 < 0x1f) {
            FUN_8005e74c(s);
            s->r0 = 0x80;
            s->g0 = 0x80;
            s->b0 = 0x80;
            t = f & 0xfff;
            f = t;
            if (DAT_800a5de4[t].flag == 0) {
                s->x0 = MESSAGE_SLOTS[o->idx].x + G.x;
                if (DAT_800a5de4[t].w >= 11)
                    s->y0 = MESSAGE_SLOTS[o->idx].y + G.y - 2;
                else
                    s->y0 = MESSAGE_SLOTS[o->idx].y + G.y;
            } else {
                s->x0 = MESSAGE_SLOTS[o->idx].x + G.x - 7;
                s->y0 = MESSAGE_SLOTS[o->idx].y + G.y - 9;
            }
            s->u0 = DAT_800a5de4[(short)f].u << 2;
            s->v0 = DAT_800a5de4[(short)f].v;
            s->w = DAT_800a5de4[(short)f].w;
            s->h = DAT_800a5de4[(short)f].h;
            s->clut = G.clut;
#ifdef PORT2
            {   /* a controller button glyph in a colour the font CLUT lacks: the port CLUT (port2/shims/prompts.c) */
                extern unsigned short port_prompt_sprite_clut(int, unsigned short);
                s->clut = port_prompt_sprite_clut(c, s->clut);
            }
#endif
            {
                Sprt *q = NEXT_PRIM;
                *q = *s;
                FUN_8005e580(CURRENT_OT + 8, q);
            }
            NEXT_PRIM = (char *)NEXT_PRIM + 0x14;
        }
    }
    if (FUN_8005efe4() != 1)
        FUN_8005efe4();
    r = NEXT_PRIM;
    FUN_80060008(r, 0, 0, 0, 0);
    FUN_8005e580(CURRENT_OT + 8, r);
    l.m = o->m;
    l.a = o->a;
    l.b = o->b;
    l.c = o->c;
    l.d = o->d;
    NEXT_PRIM = (char *)NEXT_PRIM + 0xc;
    drawBalloonFrame(&l, 0, o->v1e);
}
#endif
