// FUNC 800fb758 1140 I0NFO2
// MATCHING 800fb758 1140
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj D_800A6038;
extern TObj *D_8009F2FC;
extern TObj *D_8009F300;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDAB[];
extern unsigned char D_800A60E4;
extern void func_800F2D50(TObj *o, int a, int b);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);

void func_800FB758(TObj *o)
{
    B12 v;
    TObj *a = D_8009F2FC;
    TObj *b = D_8009F300;

    switch (o->state) {
    case 0:
        a->step = 2;
        a->state = 0;
        b->step = 2;
        b->state = 0;
        o->state = 2;
        D_800A6038.step = 4;
        D_800A6038.state = 0;
        v = *(B12 *)&a->a;
        a->d90 = (int)showMessageBoxDirect(2, 5, &v);
        func_800F2D50(a, 8, 0);
        D_8009C93F = 1;
        D_8009C942 = 1;
        break;
    case 2:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 6, &v);
            func_800F2D50(b, 8, 0);
            func_800F2D50(a, 0, 0);
            o->state++;
        }
        break;
    case 3:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 7, &v);
            func_800F2D50(a, 8, 0);
            func_800F2D50(b, 0, 0);
            o->state++;
        }
        break;
    case 4:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 8, &v);
            func_800F2D50(b, 8, 0);
            func_800F2D50(a, 0, 0);
            o->state++;
        }
        break;
    case 5:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 9, &v);
            func_800F2D50(a, 8, 0);
            func_800F2D50(b, 0, 0);
            o->state++;
        }
        break;
    case 6:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            D_8009CDAB[0]++;
            o->state++;
            func_800F2D50(a, 0, 0);
            func_800F2D50(b, 0, 0);
            setEventStarted(0x61, 0, 0);
        }
        break;
    case 7:
        a->animFrame = 1;
        b->animFrame = 1;
        func_800F2D50(a, 4, 0);
        func_800F2D50(b, 4, 0);
        o->w08 = 0xC8;
        o->state++;
        break;
    case 8:
        a->a.p.whole += 2;
        a->y.p.whole -= 2;
        b->a.p.whole += 2;
        b->y.p.whole -= 2;
        if (--o->w08 <= 0) {
            o->state++;
        }
        break;
    case 9:
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        D_800A6038.step = 1;
        D_800A6038.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
