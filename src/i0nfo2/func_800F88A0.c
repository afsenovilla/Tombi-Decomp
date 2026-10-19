// FUNC 800f88a0 1212 I0NFO2
// MATCHING 800f88a0 1212
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj D_800A6038;
extern TObj *D_8009F304;
extern void *D_800106E4;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDD6[];
extern unsigned char D_8009CEF4;
extern void func_800EEBE8(TObj *o, int a, int b);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void awardEventProgress(int, int, int);
extern void AnimLoadDuration(TObj *o);
extern void AnimAdvance(TObj *o);

void func_800F88A0(TObj *o)
{
    B12 v;
    TObj *t = D_8009F304;

    switch (o->state) {
    case 0:
        func_800EEBE8(t, 8, 0);
        t->step = 2;
        t->state = 0;
        D_800A6038.step = 4;
        D_800A6038.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        t->animFrame = t->b69 - 1;
        o->state++;
        if (D_8009CDD6[0] == 1 && D_8009CDD5 == D_8009CDD6[0]) {
            o->state = 10;
        }
        break;
    case 1:
        v = *(B12 *)&t->a;
        t->d90 = (int)showMessageBoxDirect(2, 0x5D, &v);
        func_800EEBE8(t, 8, 0);
        o->state++;
        break;
    case 2:
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            t->b68 = 0;
            func_800EEBE8(t, 0, 0);
            if (D_8009CDD6[0] == 0) {
                awardEventProgress(0x32, 0, 0);
                D_8009CDD6[0] = 1;
                o->w08 = 0x78;
            } else {
                o->w08 = 2;
            }
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            o->state = 9;
        }
        break;
    case 9:
        t->b68 = 0;
        t->b6a = 0;
        t->step = 0;
        t->state = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A6038.b04 = 1;
        D_800A6038.step = 0;
        o->step = 0;
        o->state = 0;
        break;
    case 10:
        v = *(B12 *)&t->a;
        func_800EEBE8(t, 8, 0);
        t->d90 = (int)showMessageBoxDirect(2, 0x5E, &v);
        o->state++;
        break;
    case 11:
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            t->animFrame = 1;
            func_800EEBE8(t, 5, 0);
            o->w08 = 8;
            o->state++;
            D_8009CEF4 = 1;
        }
        break;
    case 12:
        t->y.raw -= 0x18000;
        if (--o->w08 <= 0) {
            t->animFrame = 0;
            func_800EEBE8(t, 4, 0);
            o->w08 = 0x18;
            o->state++;
        }
        break;
    case 13:
        t->h->raw += 0x18000;
        if (--o->w08 <= 0) {
            t->animFrame = 0;
            func_800EEBE8(t, 4, 0);
            o->w08 = 8;
            o->state++;
        }
        break;
    case 14:
        t->y.raw += 0x18000;
        if (--o->w08 <= 0) {
            t->animFrame = 2;
            func_800EEBE8(t, 2, 0);
            o->w08 = 0x10;
            o->state++;
        }
        break;
    case 15:
        if (--o->w08 <= 0) {
            t->h->p.whole = D_800A6038.h->p.whole + 8;
            t->y.p.whole = D_800A6038.y.p.whole - 8;
            o->w08 = 0x10;
            o->state++;
        }
        break;
    case 16:
        if (--o->w08 <= 0) {
            D_800A6038.anim = D_800106E4;
            AnimLoadDuration(&D_800A6038);
            o->state++;
        }
        break;
    case 17:
        D_800A6038.h->raw -= 0x28000;
        t->h->p.whole = D_800A6038.h->p.whole + 8;
        AnimAdvance(&D_800A6038);
        break;
    }
}
