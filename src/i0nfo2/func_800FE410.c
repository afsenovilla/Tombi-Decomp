// FUNC 800fe410 1080 I0NFO2
// MATCHING 800fe410 1080
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern void func_80026C50(int a, int b, int c);
extern TObj *func_8002DCC8(int a, int b, Pos6 *p);
extern void func_80059D44(int a);
extern void func_8005A9A4(int a, int b);
extern void func_800F5590();
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93C;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009C975;
extern TObj *D_8009F2F4;
extern TObj *D_8009F2F8;
extern TObj *D_8009F2FC;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern unsigned short D_800A6066;

void func_800FE410(TObj *o)
{
    Pos6 p;
    TObj *a = D_8009F2F4;
    TObj *b = D_8009F2F8;
    TObj *c = D_8009F2FC;
    TObj *m;
    switch (o->state) {
    case 0:
        if (D_8009C93A != 0) {
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A6066 = 6;
            D_800A603C = 4;
            D_800A603D = 3;
            D_800A603E = 0;
            a->step = 2;
            a->state = 0;
            c->step = 2;
            c->state = 0;
            o->w08 = 0x78;
            o->state++;
        }
        break;
    case 1:
        if (--o->w08 <= 0) {
            a->animFrame = 2;
            p = *(Pos6 *)&a->a;
            func_800F5590(a, 8, 0);
            a->d90 = (int)func_8002DCC8(3, 0xE, &p);
            o->state++;
        }
        break;
    case 2:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            p = *(Pos6 *)&a->a;
            a->d90 = (int)func_8002DCC8(3, 0xF, &p);
            o->state++;
        }
        break;
    case 3:
        m = (TObj *)a->d90;
        if (m->b04 == 2) {
            m->b04 = 3;
            a->animFrame = 3;
            func_800F5590(a, 4, 0);
            o->w08 = 0x82;
            o->state++;
        }
        break;
    case 4:
        a->y.p.whole += 2;
        if (--o->w08 <= 0) {
            a->b04 = 3;
            D_800A6066 = 2;
            D_800A603C = 4;
            D_800A603D = 3;
            D_800A603E = 0;
            o->state++;
        }
        break;
    case 5:
        if (D_8009C975 == 0) {
            D_8009C93C = 1;
            o->w08 = 0x80;
            o->state++;
        }
        break;
    case 6:
        func_80059D44(o->w08);
        func_800F5590(b, 0xF, 0);
        b->step = 2;
        b->state = 0;
        c->_pad0e[0] = 2;
        *(signed char *)&c->b0f = -0xC;
        o->state++;
        break;
    case 7:
        o->w08 -= 0x10;
        func_80059D44(o->w08);
        if (o->w08 <= 0) {
            o->w08 = 0x78;
            o->state = 8;
        }
        break;
    case 8:
        if (--o->w08 <= 0) {
            o->w08 = 0x3C;
            b->b04 = 3;
            c->animFrame = 0;
            o->state++;
        }
        break;
    case 9:
        c->y.p.whole++;
        if (--o->w08 <= 0) {
            o->w08 = 0x3C;
            c->animFrame = 0;
            c->b04 = 3;
            func_80026C50(0x5D, 1, 1);
            o->state++;
        }
        break;
    case 10:
        if (--o->w08 <= 0) {
            o->w08 = 0xC8;
            func_8005A9A4(0xB2, 0);
            o->state++;
        }
        break;
    case 11:
        if (--o->w08 <= 0) {
            D_800A6066 = 2;
            D_8009C93C = 0;
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
