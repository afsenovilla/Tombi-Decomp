// FUNC 801163b8 516 X006
// MATCHING 801163b8 516
#include "TOBJ.H"
typedef struct { char pad[0x2c]; unsigned short w2c; } PL;
typedef struct { char pad[0x4c]; short w4c; short w4e; } SC;
extern PL *D_8009C94C;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C958;
extern short D_8009CD94;
extern short D_8009CD96;
extern short D_8009CDA0;
extern unsigned char D_8009C93C;
extern unsigned char D_8009C975;
extern SC *D_1F8001D4;
extern void func_8011610C(TObj *);
extern void func_80116254(TObj *);
extern void func_80118304(int, int, int);
extern void playSFX(int);

void func_801163B8(TObj *o)
{
    PL *p = D_8009C94C;

    switch (o->step) {
    case 0:
        o->step++;
        func_8011610C(o);
        break;
    case 1:
        if (D_8009C93A != 0) {
            o->step++;
        }
    call:
        func_8011610C(o);
        break;
    case 2:
        if (p->w2c >= 0xb0) {
            o->step++;
        }
        goto call;
    case 3:
        if ((*(int *)&o->velX += 0x100) >= 0) {
            *(int *)&o->velX = 0;
            o->step++;
        }
        goto call;
    case 4:
        if (p->w2c >= 0xc1) {
            func_80118304(5, 100, 0x46);
            playSFX(0x103);
            o->step++;
            D_8009C93F = 1;
            D_8009C93E = 1;
            D_8009C958 = 3;
        }
        goto call;
    case 5:
        if (p->w2c >= 0xc8) {
            D_8009C958 = 0;
            o->w4a = 0x6e;
            o->step++;
        }
        goto call;
    case 6:
        if (--o->w4a == -1) {
            o->step++;
            D_8009CD94 = 6;
            D_8009CD96 = 1;
            D_8009CDA0 = 0;
            D_8009C93C = 0;
            D_8009C975 = 3;
        }
        goto call;
    case 7:
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        break;
    default:
        return;
    }
    func_80116254(o);
}
