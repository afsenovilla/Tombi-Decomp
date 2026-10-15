// FUNC 800ee414 400 I0NFO3
#include "TOBJ.H"
typedef struct { short a, x, b, y, c, d; } BoxXY;
extern int func_8002DCC8(int a, int b, BoxXY *p);
extern void func_8005A9A4(int e, int a);
extern unsigned char D_8009C93F;
extern unsigned char D_8009C940[];
extern unsigned char D_8009C941;
extern unsigned char D_8009C942;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern TObj *D_800A60C8;
extern unsigned char D_800A60E4;

static __inline__ short usingItem(unsigned char *p)
{
    short r = 0;
    if (p[0] != 0) {
        r = D_8009C941 == 0x95;
    }
    return r;
}

void func_800EE414(TObj *o)
{
    BoxXY b;
    register unsigned char *u asm("$6");
    switch (o->state) {
    case 0:
        u = D_8009C940;
        if (usingItem(u)) {
            b.x = 0x90;
            b.y = 0x40;
            D_800A603D = 2;
            D_800A603E = 0;
            u[-1] = 1;
            D_8009C942 = 1;
            u[0] = 0;
            D_800A60C8 = (TObj *)func_8002DCC8(6, 0, &b);
            o->state++;
            break;
        }
        goto done;
    case 1:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            func_8005A9A4(0x7D, 0);
            o->w08 = 0xC8;
            o->state = 9;
        }
        break;
    case 9:
        if (--o->w08 <= 0) {
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
        done:
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
