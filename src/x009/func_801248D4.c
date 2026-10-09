// FUNC 801248d4 924 X009
// MATCHING 801248d4 924
#include "TOBJ.H"

extern unsigned char D_8009C964, D_8009C93A, D_800A603B, D_8009D2B1;
extern int DAT_1f8002d4[];
extern void *D_8012E004[];
extern int FUN_800202b4(TObj *);
extern void func_801174C4(TObj *);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026c50(int, int, int);
extern void func_80124C70(TObj *);
extern void FUN_80018790(TObj *);

void func_801248D4(TObj *o)
{
    unsigned char s;

    switch (o->b04) {
    case 0:
        if (o->subtype == 4) {
            o->ba7 = 2;
            o->box0 = 0xc;
            o->box1 = 0x18;
            o->box2 = 0xe;
            o->box3 = 0x1c;
        } else {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0xa;
            o->box3 = 0x14;
            o->ba7 = o->animFrame;
            *(int *)&o->w5c = o->a.p.whole;
            o->d60 = o->y.p.whole;
            o->d64 = o->b.p.whole;
        }
        *(signed char *)&o->b0f = -2;
        o->step = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->animFrame = 0;
        o->b0d = 0;
        o->b0a = 0;
        o->w1e = 9;
        o->b04++;
        o->d3c = DAT_1f8002d4[0];
        o->anim = D_8012E004[o->b0c];
        if (o->ba7 != 1) {
            o->d30 = o->a.raw;
            o->d34 = o->y.raw;
            o->d38 = o->b.raw;
        }
        break;
    case 1:
        if (D_8009C964 != 0x20 && D_8009C93A == 1)
            break;
        if (o->ba7 == 1)
            func_801174C4(o);
        FUN_800202b4(o);
        break;
    case 2:
        if (o->subtype == 4) {
            switch (o->step) {
            case 0:
            case 1:
                FUN_800202b4(o);
                break;
            case 2:
                FUN_800202b4(o);
                D_800A603B = 1;
                FUN_8005a9a4(0xbd, 0);
                o->b04 = 3;
                break;
            }
            break;
        }
        switch (s = o->step) {
        case 0:
            FUN_800202b4(o);
            if (o->ba7 == 1)
                func_801174C4(o);
            break;
        case 1:
            FUN_800202b4(o);
            if (o->subtype == 3)
                break;
            if (o->ba7 == s) {
                ((TObj *)o->d90)->b6a = o->subtype;
                break;
            }
            ((TObj *)o->d90)->b6a = o->subtype;
            if (o->d94 != 0)
                ((TObj *)o->d94)->b6a = o->subtype;
            break;
        case 2:
            FUN_800202b4(o);
            if (o->subtype != 3) {
                D_8009D2B1 = o->subtype;
                if (o->ba7 == 1)
                    ((TObj *)o->d90)->b6a = o->subtype;
            }
            o->step = 3;
            o->state = 0;
            break;
        case 3:
            if (o->subtype == 3) {
                FUN_80026c50(9, 1, 1);
                o->b04 = 3;
                break;
            }
            if (D_8009C964 != 0x21)
                func_80124C70(o);
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
