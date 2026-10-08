// FUNC 800ed750 1212 X003
// MATCHING 800ed750 1212
#include "TOBJ.H"
typedef struct { short spd[2]; short pos[2]; short cnt[2]; } B;
typedef struct { int n; char *b8; int tab[4]; short val[2]; } E;
extern int **DAT_80114c70[];
extern char DAT_80114c7c[];
extern char D_800E3E28[];
extern void FUN_80025aa8(int, char *);
extern void FUN_80024ea0(char *, int, int);
extern void FUN_8003c7c4(void *d, void *arg, short x, short y, int flag);

#define INIT(o, b)                       \
    if (o->animFrame == 0) {             \
        o->timer = -10;                  \
        o->w22 = 0;                      \
    } else if (o->animFrame == 1) {      \
        o->timer = 0;                    \
        o->w22 = -10;                    \
    }                                    \
    b->pos[0] = 0x800;                   \
    b->pos[1] = 0x800;                   \
    b->spd[0] = 0x200;                   \
    b->spd[1] = 0x200;                   \
    b->cnt[0] = 1;                       \
    b->cnt[1] = 1;

void FUN_800ed750(TObj *o)
{
    E *e = (E *)&o->wb4;
    B *b = (B *)&o->w48;
    int **pp;
    int *p;
    short i;
    short s;
    int a, lim;
    Fix16 pos[3];

    switch (o->state) {
    case 0:
        INIT(o, b);
        pp = DAT_80114c70[o->b0c];
        p = *pp;
        e->n = *p++;
        for (i = 0; i < e->n; i++) {
            e->tab[i] = (int)*pp + p[i];
        }
        e->b8 = D_800E3E28 + o->b0c * 0x1600;
        *(char **)&o->wa8 = e->b8;
        FUN_80025aa8(o->da0, e->b8);
        o->state = 1;
        if (o->b68 == 2) o->w98 = 1;
        o->b68 = 0;
        o->b0a = 0x12;
        break;
    case 1:
        if (o->w98 == 1) {
            s = 0x100;
            if (o->animFrame) s = -0x100;
            if (o->subtype == 8) s = s / 2;
            pos[0].p.whole = o->h->p.whole;
            pos[1].p.whole = o->y.p.whole - 0x50;
            pos[2].p.whole = o->d->p.whole;
            FUN_8003c7c4(DAT_80114c7c + o->subtype * 6, pos, s, -0x200, 1);
            o->w98 = 0;
        }
        if (o->visible == 0) goto set2;
        if (o->b69) {
            o->state = 2;
        } else if (o->b68) {
            o->b68 = 0;
            INIT(o, b);
        }
        FUN_80025aa8(o->da0, e->b8);
        for (i = 0; i < e->n; i++) {
            if ((&o->timer)[i] >= 0) {
                if (b->cnt[i] < 4) {
                    e->val[i] = b->pos[i];
                    a = b->spd[i] / b->cnt[i];
                    b->pos[i] += a;
                    lim = 0x1000 / b->cnt[i];
                    if (lim < b->pos[i]) {
                        b->pos[i] = lim;
                        b->spd[i] *= -1;
                    } else if (b->pos[i] < 0) {
                        b->pos[i] = 0;
                        b->spd[i] *= -1;
                        b->cnt[i]++;
                    }
                }
            } else {
                e->val[i] = 0;
            }
            (&o->timer)[i]++;
            FUN_80024ea0(e->b8, e->tab[i], e->val[i]);
        }
        if (b->cnt[0] < 4) break;
        if (b->cnt[1] < 4) break;
    set2:
        o->state = 2;
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        o->b0a = o->ba7;
        break;
    }
}
