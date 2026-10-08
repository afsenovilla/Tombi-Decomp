// FUNC 8011c574 2388 X001
// MATCHING 8011c574 2388
#include "TOBJ.H"

typedef struct {
    short x0, x2, x4, x6, x8, xa, xc, n;
    short w10, w12, w14, w16, w18;
    unsigned char b1a, b1b;
    short w1c, w1e, w20;
} Seg;

typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe;
} Ctx;

typedef struct { short x, y; } P;

typedef struct { char pad[0xb4]; unsigned short wb4, wb6, wb8, wba, wbc, wbe, wc0, wc2; } Ext;
#define X(o) ((Ext *)(o))

extern Seg D_800A4470[];
extern Seg D_8013C498[];
extern char D_8013C490[];
extern Ctx D_800A64C0;
extern short D_800A64C6[];
extern Fix16 *D_800A6078;
extern short D_800A604E;
extern unsigned char D_800A60D4;
extern int MulCos(int, short);
extern int MulNegSinScaled(int, short);
extern TObj *FUN_800184d8(void);
extern TObj *func_80020D98(void *);
extern int FUN_800201ac(TObj *, int);
extern int func_800203DC(TObj *);
extern int FUN_8002078c(P, P);
extern void func_8011CEC8(Ctx *);
extern void func_8011D28C(TObj *);
extern void playSFX(int);
extern void ObjFreeDup(TObj *);

void func_8011C574(TObj *o)
{
    Ctx *s1;
    TObj *c;
    TObj *prev;
    TObj *q;
    int j, k, m;
    short dx, dy;
    P r, p;

    switch (o->b04) {
    case 0:
        for (k = 0; k < 6; k++) {
            D_800A4470[k] = D_8013C498[k];
        }
        s1 = &D_800A64C0;
        o->ba5 = 0x10;
        o->ba6 = 8;
        if (o->d90 == 0) {
            prev = o;
            for (j = 0; j < 6; j++) {
                D_800A4470[j].x4 = D_800A4470[j].xa;
                D_800A4470[j].x2 = D_800A4470[j].x8 - MulCos(D_800A4470[j].b1a, D_800A4470[j].xc);
                D_800A4470[j].x0 = D_800A4470[j].x6 + MulNegSinScaled(D_800A4470[j].b1a, D_800A4470[j].xc);
                for (k = 0; k < D_800A4470[j].n; k++) {
                    if (k == 0 && j == 0) {
                        o->b0a = 0x11;
                        o->b0c = 0;
                        o->b0f = 0;
                        o->active = 2;
                        s1->f0 = 0;
                        s1->f2 = D_800A4470[0].n;
                        s1->f4 = 0;
                        s1->f6 = -1;
                        s1->f8 = 0;
                        func_8011CEC8(s1);
                        o->a.raw = (D_800A4470[0].x4 + s1->fe) << 16;
                        o->y.raw = (D_800A4470[0].x2 + s1->fc) << 16;
                        o->b.raw = (D_800A4470[0].x0 + s1->fa) << 16;
                        X(o)->wb4 = D_800A4470[0].x2 + s1->fc;
                        X(o)->wb6 = D_800A4470[0].x2 + s1->fc;
                        X(o)->wb8 = 0;
                        X(o)->wba = 0;
                        X(o)->wc2 = 0xffff;
                        continue;
                    }
                    c = FUN_800184d8();
                    if (c != 0) {
                        c->da0 = o->da0;
                        c->category = 4;
                        c->active = 1;
                        c->type = 0xf;
                        c->ba4 = 0;
                        c->b6b = 0;
                        c->subtype = j;
                        c->b0c = k;
                        c->animFrame = 0;
                        c->b0a = 0x11;
                        c->b0f = 0;
                        s1->f0 = k;
                        s1->f2 = D_800A4470[j].n;
                        s1->f4 = j;
                        s1->f6 = -1;
                        s1->f8 = 0;
                        func_8011CEC8(s1);
                        c->a.raw = (D_800A4470[j].x4 + s1->fe) << 16;
                        c->y.raw = (D_800A4470[j].x2 + s1->fc) << 16;
                        c->b.raw = (D_800A4470[j].x0 + s1->fa) << 16;
                        c->d90 = (int)prev;
                        prev->d94 = (int)c;
                        X(c)->wb4 = D_800A4470[j].x2 + s1->fc;
                        X(c)->wb6 = D_800A4470[j].x2 + s1->fc;
                        prev = c;
                        X(c)->wb8 = 0;
                        X(c)->wba = 0;
                        X(c)->wbc = c->a.p.whole;
                        X(c)->wbe = c->y.p.whole;
                        X(c)->wc0 = c->b.p.whole;
                    }
                    if (j == 3 && k == D_800A4470[3].n - 1) {
                        q = func_80020D98(D_8013C490);
                        if (q != 0) {
                            q->active = 1;
                            q->a.raw = c->a.raw;
                            q->y.raw = c->y.raw;
                            q->b.raw = c->b.raw;
                            q->d90 = (int)c;
                        }
                    }
                }
            }
            c->d94 = 0;
        }
        o->b04++;
        break;
    case 1:
        s1 = &D_800A64C0;
        if (o->d90 == 0) {
            D_800A64C6[0] = -1;
            for (c = (TObj *)o->d94; c != 0; c = (TObj *)c->d94) {
                if (c->b69 == 0) {
                    if (c->subtype < 4) continue;
                    if (D_800A6078->p.whole < 0x5b0) continue;
                }
                if (c->b0c < 3) continue;
                if (D_800A4470[c->subtype].n - c->b0c < 4) continue;
                s1->f6 = c->subtype;
                if (s1->f6 != X(o)->wc2) {
                    for (m = 0xaa; m >= 0; m -= 0x22) {
                        *(short *)((char *)D_800A4470 + m + 0x1e) = 1;
                    }
                }
                s1->f8 = c->b0c;
                func_8011D28C(c);
                break;
            }
            X(o)->wc2 = s1->f6;
        }
        FUN_800201ac(o, 0xa0);
        if (o->visible == 0) break;
        if (func_800203DC(o) == 0) o->visible = 0;
        if (o->visible == 0) break;
        s1->f0 = o->b0c;
        s1->f2 = D_800A4470[o->subtype].n;
        s1->f4 = o->subtype;
        func_8011CEC8(s1);
        X(o)->wb6 = D_800A4470[o->subtype].x2 + s1->fc;
        if (X(o)->wb4 != X(o)->wb6) {
            X(o)->wb4 += (X(o)->wb6 - X(o)->wb4) >> 2;
        }
        o->h->raw = (D_800A4470[o->subtype].x0 + s1->fa) << 16;
        o->y.raw = X(o)->wb4 << 16;
        dx = o->h->p.whole - X(o)->wbc;
        dy = o->y.p.whole - X(o)->wbe;
        X(o)->wbc = o->h->p.whole;
        X(o)->wbe = o->y.p.whole;
        if (s1->f6 != -1 && o->subtype == s1->f6) {
            if (o->b69 != 0) {
                if (X(o)->wb8 == 0) {
                    X(o)->wb8++;
                    if (X(o)->wba == 0) playSFX(0x4d); else playSFX(0x4e);
                    X(o)->wba ^= 1;
                }
                if (D_800A60D4 == 0) {
                    *(short *)((char *)D_800A6078 + 2) += dx;
                    D_800A604E += dy;
                }
                o->b69 = 0;
            } else {
                X(o)->wb8 = 0;
            }
        }
        r.x = o->h->p.whole;
        r.y = o->y.p.whole;
        c = (TObj *)o->d94;
        if (c != 0) {
            p.x = c->h->p.whole;
            p.y = c->y.p.whole;
        } else {
            p.x = o->h->p.whole + 0x10;
            p.y = o->y.p.whole;
        }
        o->d8c = FUN_8002078c(r, p) << 4;
        o->d30 = p.x;
        o->d34 = p.y;
        o->box0 = 0;
        o->box2 = r.y - p.y;
        if (o->box2 < 0) o->box2 = 0;
        o->box1 = p.x - r.x;
        o->box3 = p.y - r.y + 12;
        if (o->box3 < 12) o->box3 = o->box2 + 12;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
