// FUNC 8011fd7c 576 X001
// MATCHING 8011fd7c 576
#include "TOBJ.H"

typedef struct { Fix16 x, y, z; } V3;
extern int D_1F8002D4[];
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern void *D_8013E5CC[];
extern char D_8013C760[];
extern unsigned char D_8009CEAE;
void ObjCullRegister(TObj *o);
void func_80118998(short x, short y, short z);
void FUN_8002ee50(int a, int x, int y, int z);
void FUN_8003c7c4(char *, V3 *, int, int, int);
void FUN_80020aec(int a);
void ObjFreeDup(TObj *o);

void func_8011FD7C(TObj *o)
{
    V3 v;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0xb;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8013E5CC[o->subtype];
        o->box0 = 0x10;
        o->box1 = 0x1c;
        o->box2 = 0xe;
        o->box3 = 0x1c;
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        switch (o->step) {
        case 0:
            o->step++;
            ObjCullRegister(o);
            func_80118998(o->h->p.whole, o->y.p.whole, o->d->p.whole);
            FUN_8002ee50(2, o->h->p.whole, o->y.p.whole, o->d->p.whole);
            v.x.p.whole = o->h->p.whole;
            v.y.p.whole = o->y.p.whole;
            v.z.p.whole = o->d->p.whole;
            FUN_8003c7c4(D_8013C760, &v, 0, 0, 0);
            o->timer = 0x20;
            D_8009CEAE = 4;
            break;
        case 1:
            if (--o->timer == -1) {
                o->step++;
            } else if ((D_1F8001F8 + D_1F800198) & 1) {
                ObjCullRegister(o);
            }
            break;
        case 2:
            o->b04 = 3;
            ((TObj *)o->d94)->b04 = 3;
            FUN_80020aec(o->b6b);
            FUN_80020aec(((TObj *)o->d94)->b6b);
            break;
        }
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
