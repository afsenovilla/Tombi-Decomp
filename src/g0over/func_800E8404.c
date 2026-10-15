// FUNC 800e8404 924 G0OVER
// MATCHING 800e8404 924
#include "TOBJ.H"
typedef struct { short m[3][3]; int t[3]; } MATRIX;
extern short D_800E8CB4[];
extern short D_800E8CC4[];
typedef struct {
    MATRIX m;       /* 0x00 */
    MATRIX cm;      /* 0x20 */
    unsigned char r, g, b, p43;
    short s44, s46, s48;
} OB;
extern OB D_1F800118;
typedef struct { unsigned char r, g, b; } COL;
extern COL D_1F800158;
extern MATRIX D_1F800138;
extern MATRIX D_1F800138_cm; /* the same matrix */
extern TObj *D_1F8001D4;
extern void func_800E82F8(TObj *o);
extern void SetBackColor(int r, int g, int b);
extern void SetColorMatrix(MATRIX *m);

void func_800E8404(TObj *o, int hold)
{
    unsigned char s = o->b04;
    switch (s) {
    case 200:
        break;
    case 0:
        o->b04 = s + 1;
        o->a.p.whole = D_800E8CB4[o->type];
        o->y.p.whole = D_800E8CC4[o->type];
        o->b.p.whole = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->subtype = o->type % 3;
        o->d8c = 0;
        o->timer = 0x18;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (--o->timer == -1) {
                o->timer = 8;
                o->step++;
            } else {
                func_800E82F8(o);
                o->b.p.whole += 0x28;
            }
            break;
        case 1:
            if (--o->timer == -1) {
                o->timer = 0xC;
                o->step++;
            }
            break;
        case 2:
            if (--o->timer == -1) {
                o->timer = 0x78;
                o->step++;
            } else {
                o->b.p.whole -= 0x50;
            }
            break;
        case 3:
            if (hold == 0) {
                if (--o->timer == -1) {
                    o->step++;
                }
            } else if (*(short *)((char *)D_1F8001D4 + 0x5C) < 0x3D) {
                o->step += 2;
            }
            break;
        case 4:
            switch (o->subtype) {
            case 0:
                o->y.raw += 0x50000;
                o->d84 += 0x28;
                o->d88 -= 0x38;
                break;
            case 1:
                o->y.raw += 0x20000;
                o->d84 += 0x40;
                o->d88 += 0x20;
                break;
            case 2:
                o->y.raw += 0x38000;
                o->d84 += 0x38;
                o->d88 += 0x28;
                break;
            }
            break;
        case 5:
            if (o->type == 0) {
                COL *ob = &D_1F800158;
                if (ob->r != 0) {
                    ob->r -= 2;
                    ob->g -= 2;
                    ob->b -= 2;
                    SetBackColor(ob->r, ob->g, ob->b);
                }
                if (D_1F800138.m[0][0] > 0) {
                    D_1F800118.cm.m[0][0] = D_1F800138.m[0][0] - 0x48;
                    D_1F800118.cm.m[1][0] -= 0x48;
                    D_1F800118.cm.m[2][0] -= 0x48;
                } else {
                    D_1F800118.cm.m[0][0] = 0;
                    D_1F800118.cm.m[1][0] = 0;
                    D_1F800118.cm.m[2][0] = 0;
                }
                SetColorMatrix(&D_1F800118.cm);
            }
            break;
        }
        break;
    }
}
