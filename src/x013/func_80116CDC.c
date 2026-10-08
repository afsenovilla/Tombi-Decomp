// FUNC 80116cdc 772 X013
// MATCHING 80116cdc 772
#include "TOBJ.H"

extern unsigned char D_8009C940[];
extern unsigned char D_8009C941;
extern unsigned short D_8009C960[];
extern void *D_8013B0D8;
extern void *D_80134D84;
extern void *D_80131CE0;
extern int D_1F8002D4[];
extern unsigned char D_8009CD98[], D_8009CD9C[], D_800B146C[], D_800B1474[], D_800B1470[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_8005a9a4(int, int);
extern void FUN_800188e0(TObj *);

void func_80116CDC(TObj *o)
{
    int v;
    int k;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (D_8009C940[0] != 0) {
                if ((v = D_8009C941) == 0x7e) {
                    D_8009C940[0] = 0;
                    v = 8;
                    o->box0 = v;
                    o->box1 = 0x10;
                    o->box2 = v;
                    o->box3 = 0x10;
                    o->active = 1;
                    o->b69 = 0;
                    o->b0a = 0;
                    switch (D_8009C960[0]) {
                    case 0:
                        o->b0d = 1;
                        o->w1e = 6;
                        o->w08 = 0x7c0f;
                        o->anim = D_8013B0D8;
                        o->d3c = D_1F8002D4[0];
                        break;
                    case 4:
                        o->b0d = 1;
                        o->w1e = 9;
                        o->w08 = 0x7a0e;
                        o->anim = D_80134D84;
                        o->d3c = D_1F8002D4[0];
                        break;
                    case 10:
                        o->b0d = 1;
                        o->w1e = 0xc;
                        o->w08 = 0x7acc;
                        o->anim = D_80131CE0;
                        o->d3c = D_1F8002D4[0];
                        break;
                    }
                    FUN_8001fe6c(o);
                    o->b04++;
                }
            } else {
                o->active = 2;
            }
            break;
        case 1:
            v = 8;
            o->box0 = v;
            o->box2 = v;
            o->active = 1;
            o->b69 = 0;
            o->b0a = 0;
            o->box1 = 0x10;
            o->b0d = 1;
            o->box3 = 0x10;
            o->w1e = 6;
            o->w08 = 0x7c0f;
            o->anim = D_8013B0D8;
            o->d3c = D_1F8002D4[0];
            FUN_8001fe6c(o);
            break;
        }
        break;
    case 1:
        if (FUN_800202b4(o))
            FUN_8001fec0(o);
        break;
    case 2:
        if (o->subtype == 0) {
            FUN_8005a9a4(0x77, 0);
            switch (D_8009C960[0]) {
            case 0:
                k = 0;
                break;
            case 10:
                k = 2;
                break;
            case 4:
            case 12:
                k = 1;
                break;
            }
            D_8009CD98[k] = 9;
            D_8009CD9C[k] = 0x3c;
            D_800B146C[k] = 0x3c;
            D_800B1474[k] = 1;
            D_800B1470[k] = 0x1e;
            o->b04++;
        }
    case 3:
        FUN_800188e0(o);
        break;
    }
}
